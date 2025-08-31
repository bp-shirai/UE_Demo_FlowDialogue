// Copyright Zachary Brett, 2024. All rights reserved.

//Header
#include "Nodes/DialogueSpeechNode.h"
//Plugin
#include "Dialogue.h"
#include "DialogueSpeakerComponent.h"
#include "LogDialogueTree.h"
#include "Interfaces/DialogueCharacter.h"
#include "Sound/SoundCue.h"
#include "Transitions/DialogueTransition.h"

void UDialogueSpeechNode::InitSpeechData(FSpeechDetails& InDetails,
	TSubclassOf<UDialogueTransition> TransitionType)
{
	check(TransitionType);
	Details = InDetails;
	Transition = NewObject<UDialogueTransition>(this, TransitionType);
	Transition->SetOwningNode(this);
}

FSpeechDetails UDialogueSpeechNode::GetDetails() const
{
	return Details;
}

UDialogueSpeakerComponent* UDialogueSpeechNode::GetSpeaker() const
{
	return Dialogue->GetSpeaker(Details.SpeakerName);
}

bool UDialogueSpeechNode::GetCanSkip() const
{
	return Details.bCanSkip;
}

void UDialogueSpeechNode::SelectOption(int32 InOptionIndex)
{
	Transition->SelectOption(InOptionIndex);
}

void UDialogueSpeechNode::EnterNode()
{
	//Play all events
	PlayEvents();

	//Verify speaker is actually present
	if (!Dialogue->SpeakerIsPresent(Details.SpeakerName))
	{
		UE_LOG(
			LogDialogueTree,
			Error,
			TEXT("Terminating dialogue early: A participant speaker component was not found. Verify that the dialogue name property matches the speaker's role in the dialogue.")
		);

		Dialogue->EndDialogue();
		return;
	}

	if (!Details.bIgnoreContent && !Details.SpeechVariations.IsEmpty())
	{
		const int SpeechVariationIndex = Details.SpeechVariations.Num() > 1 ? FMath::RandRange(0, Details.SpeechVariations.Num() - 1) : 0;
		//Display the current speech
		Dialogue->DisplaySpeech(Details, SpeechVariationIndex);

		//Play any audio and set any flags for the speaker
		StartAudio(SpeechVariationIndex);
	}
	
	//If no transition, throw an error and close the dialogue 
	if (!Transition)
	{
		UE_LOG(
			LogDialogueTree, 
			Error, 
			TEXT("Speech node is missing transition.")
		);
		Dialogue->EndDialogue();
		return; 
	}

	// G2VS2 start
	if (auto DialogueCharacter = Cast<IDialogueCharacter>(GetSpeaker()->GetOwner()))
	{
		DialogueCharacter->StopDialogueGesture();
		if (Details.GestureTag_Obsolete.IsValid())
		{
			if (FMath::RandRange(0.f, 1.f) < Details.GestureChance_Obsolete)
				DialogueCharacter->StartDialogueGesture(Details.GestureTag_Obsolete, {});
		}
	}

	for (const auto& Gesture : Details.Gestures)
	{
		auto SpeakerComponent = Dialogue->GetSpeaker(Gesture.SpeakerName);
		if (SpeakerComponent == nullptr)
			continue;

		auto DialogueCharacter = Cast<IDialogueCharacter>(SpeakerComponent->GetOwner());
		DialogueCharacter->StopDialogueGesture();
		if (FMath::RandRange(0.f, 1.f) <= Gesture.GestureChance)
		{
			FGameplayTag GestureTag;
			if (!Gesture.GestureVariations.IsEmpty())
			{
				if (Gesture.GestureVariations.Num() == 1)
				{
					GestureTag = Gesture.GestureVariations.First();
				}
				else
				{
					const TArray<FGameplayTag>& GestureOptionsArray = Gesture.GestureVariations.GetGameplayTagArray();
					GestureTag = GestureOptionsArray[FMath::RandRange(0, GestureOptionsArray.Num() - 1)];
				}
			}
			else
			{
				GestureTag = Gesture.GestureTag_Obsolete;
			}

			if (GestureTag.IsValid())
				DialogueCharacter->StartDialogueGesture(GestureTag, Gesture.SpeechGestureItems);
		}
	}
	
	// G2VS2 end
	//Play the transition 
	Transition->StartTransition();
}

void UDialogueSpeechNode::Skip()
{
	if (Details.bCanSkip)
	{
		Super::Skip();
		Transition->Skip();
		// G2VS2
		// 08.11.2024 @AK: the UDialogueSpeechNode now has events on skip, so now we just gotta do the UDialogueEvent_SkipGesture
		// to remove redundant intervention into plugins source
		if (Details.GestureTag_Obsolete.IsValid())
		{
			auto DialogueCharacter = Cast<IDialogueCharacter>(GetSpeaker()->GetOwner());
			DialogueCharacter->StopDialogueGesture();
		}
		// G2VS2
	}
}

TSubclassOf<UDialogueTransition> UDialogueSpeechNode::GetTransitionType() const
{
	return Transition->GetClass();
}

void UDialogueSpeechNode::TransitionIfNotBlocking() const
{
	Transition->CheckTransitionConditions();
}

FDialogueOption UDialogueSpeechNode::GetAsOption()
{
	return FDialogueOption{ Details, this };
}

void UDialogueSpeechNode::StartAudio(int SpeechVariationIndex)
{
	UDialogueSpeakerComponent* Speaker = GetSpeaker();

	if (Speaker)
	{
		//Play any audio
		Speaker->Stop();

		if (Details.SpeechVariations[SpeechVariationIndex].SpeechAudio)
		{
			Speaker->PlaySpeechAudioClip(Details.SpeechVariations[SpeechVariationIndex].SpeechAudio);
		}

		//Set any behavior flags
		Speaker->SetCurrentGameplayTags(Details.GameplayTags);
	}
}
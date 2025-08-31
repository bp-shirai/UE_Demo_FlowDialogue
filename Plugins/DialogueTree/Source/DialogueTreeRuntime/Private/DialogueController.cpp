// Copyright Zachary Brett, 2024. All rights reserved.

//Header
#include "DialogueController.h"
//Plugin
#include "Dialogue.h"
#include "DialogueSpeakerComponent.h"
#include "LogDialogueTree.h"
//Engine
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"


// Sets default values
ADialogueController::ADialogueController()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	bIsSpatiallyLoaded = false;
#endif
}

void ADialogueController::SelectOption(int32 InOptionIndex) const
{
	if (CurrentDialogue)
	{
		CurrentDialogue->SelectOption(InOptionIndex);
	}
}

TMap<FName, UDialogueSpeakerComponent*> ADialogueController::GetSpeakers() const
{
	if (CurrentDialogue)
	{
		return CurrentDialogue->GetAllSpeakers();
	}

	return TMap<FName, UDialogueSpeakerComponent*>();
}

void ADialogueController::StartDialogueWithNames(UDialogue* InDialogue, TMap<FName, UDialogueSpeakerComponent*> InSpeakers, bool bResume)
{
	if (!InDialogue)
	{
		UE_LOG(LogDialogueTree, Error, TEXT("Could not start dialogue. Provided dialogue null."));
		return;
	}

	if (InDialogue->GetCompileStatus() != EDialogueCompileStatus::Compiled)
	{
		UE_LOG(LogDialogueTree, Error, TEXT("Could not start dialogue [%s]. Dialogue not compiled."), *InDialogue->GetName());
		return;
	}

	if (!CanOpenDisplay())
	{
		UE_LOG(LogDialogueTree, Error, TEXT("Could not start dialogue. CanOpenDisplay() was false."));
		return;
	}

	//Set the target dialogue 
	CurrentDialogue = InDialogue;

	//Get start node 
	FName StartNodeID = CurrentDialogue->GetRootNode()->GetNodeID();
	FName DialogueName = CurrentDialogue->GetFName();
	if (bResume && DialogueHistories.Histories.Contains(DialogueName))
	{
		const auto& DialogueHistory = DialogueHistories.Histories[DialogueName];
		for (const auto& Speaker : InSpeakers)
		{
			const auto* CharacterDialogueHistory = DialogueHistory.DialogueNodeHistory.Find(Speaker.Value->GetDialogueSpeakerId());
			if (CharacterDialogueHistory)
			{
				if (!CharacterDialogueHistory->ResumeNodeID.IsNone() && CurrentDialogue->HasNode(CharacterDialogueHistory->ResumeNodeID))
				{
					StartNodeID = CharacterDialogueHistory->ResumeNodeID;
					break;
				}
			}
		}
		
	}

	for (auto& DialogueParticipant : InSpeakers)
	{
		DialogueParticipant.Value->OnDialogueStarted(CurrentDialogue);
	}
	
	//Start the dialogue 
	OpenDisplay();

	OnDialogueStarted.Broadcast();
	CurrentDialogue->OpenDialogueAt(StartNodeID, this, InSpeakers);
}

void ADialogueController::StartDialogue(UDialogue* InDialogue, TArray<UDialogueSpeakerComponent*> InSpeakers, bool bResume)
{
	if (InSpeakers.IsEmpty())
	{
		UE_LOG(LogDialogueTree, Warning, TEXT("No speakers provided on dialogue start."));
		return;
	}
	
	int CurrentParticipantIndex = 1;
	TMap<FName, UDialogueSpeakerComponent*> TargetSpeakers;
	for (UDialogueSpeakerComponent* Speaker : InSpeakers)
	{
		if (Speaker == nullptr)
		{
			UE_LOG(LogDialogueTree, Error, TEXT("Could not start dialogue. Invalid speaker provided."));
			return;
		}

		if (Speaker->GetDialogueName().IsNone())
		{
			UE_LOG(LogDialogueTree, Error, TEXT("Could not start dialogue. A provided speaker has an unfilled DialogueName."));
			return;
		}

		FName CurrentSpeakerDialogueName = Speaker->GetDialogueName();
		if (InDialogue->bUseGenericSpeakerNames)
		{
			CurrentSpeakerDialogueName = FName(FString::Printf(TEXT("Speaker%d"), CurrentParticipantIndex++));
		}
		else if (TargetSpeakers.Contains(CurrentSpeakerDialogueName))
		{
			int SameNameOccurences = 0;
			FString CurrentSpeakerDialogueNameString = CurrentSpeakerDialogueName.ToString();
			for (const auto& TargetSpeaker : TargetSpeakers)
				if (TargetSpeaker.Key.ToString().Contains(CurrentSpeakerDialogueNameString))
					SameNameOccurences++;

			CurrentSpeakerDialogueName = FName(FString::Printf(TEXT("%s%d"), *CurrentSpeakerDialogueNameString, SameNameOccurences+1));
		}
		
		TargetSpeakers.Add(CurrentSpeakerDialogueName, Speaker);
	}

	StartDialogueWithNames(InDialogue, TargetSpeakers, bResume);
}

void ADialogueController::StartDialogueWithNamesAt(UDialogue* InDialogue, FName NodeID, TMap<FName, UDialogueSpeakerComponent*> InSpeakers)
{
	if (!InDialogue)
	{
		UE_LOG(
			LogDialogueTree,
			Error,
			TEXT("Could not start dialogue. Provided dialogue null.")
		);
		return;
	}

	if (InDialogue->GetCompileStatus() != EDialogueCompileStatus::Compiled)
	{
		UE_LOG(
			LogDialogueTree,
			Error,
			TEXT("Could not start dialogue [%s]. Dialogue not compiled."),
			*InDialogue->GetName()
		);
		return;
	}

	if (!CanOpenDisplay())
	{
		UE_LOG(
			LogDialogueTree,
			Error,
			TEXT("Could not start dialogue. Display could not be opened.")
		);
		return;
	}

	if (!InDialogue->HasNode(NodeID))
	{
		UE_LOG(
			LogDialogueTree,
			Error,
			TEXT("Could not start dialogue from Node: %s. No such node exists."),
			*NodeID.ToString()
		);
		return;
	}

	CurrentDialogue = InDialogue;

	OpenDisplay();
	CurrentDialogue->OpenDialogueAt(NodeID, this, InSpeakers);
	OnDialogueStarted.Broadcast();
}

void ADialogueController::StartDialogueAt(UDialogue* InDialogue, FName NodeID, TArray<UDialogueSpeakerComponent*> InSpeakers)
{
	if (InSpeakers.IsEmpty())
	{
		UE_LOG(
			LogDialogueTree,
			Warning,
			TEXT("No speakers provided on dialogue start.")
		);
		return;
	}

	TMap<FName, UDialogueSpeakerComponent*> TargetSpeakers;
	for (UDialogueSpeakerComponent* Speaker : InSpeakers)
	{
		if (Speaker == nullptr)
		{
			UE_LOG(
				LogDialogueTree,
				Error,
				TEXT("Could not start dialogue. Invalid speaker provided.")
			);
			return;
		}

		if (Speaker->GetDialogueName().IsNone())
		{
			UE_LOG(
				LogDialogueTree,
				Error,
				TEXT("Could not start dialogue. A provided speaker has an"
					" unfilled DialogueName.")
			);
			return;
		}

		if (!InDialogue->HasNode(NodeID))
		{
			UE_LOG(
				LogDialogueTree,
				Error,
				TEXT("Could not start dialogue from Node: %s. No such node exists."),
				*NodeID.ToString()
			);
			return;
		}

		if (TargetSpeakers.Contains(Speaker->GetDialogueName()))
		{
			UE_LOG(
				LogDialogueTree,
				Error,
				TEXT("Could not start dialogue. Multiple provided speakers "
					"share a DialogueName.")
			);
			return;
		}

		TargetSpeakers.Add(Speaker->GetDialogueName(), Speaker);
	}

	StartDialogueWithNamesAt(InDialogue, NodeID, TargetSpeakers);
}

void ADialogueController::EndDialogue()
{
	CloseDisplay();
	OnDialogueEnded.Broadcast();

	if (CurrentDialogue)
	{
		//Clear any behavior flags from the speakers and stop speaking
		for (auto& Entry : CurrentDialogue->GetAllSpeakers())
		{
			if (Entry.Value)
			{
				Entry.Value->OnDialogueEnded(CurrentDialogue);
				Entry.Value->Stop();
				Entry.Value->ClearGameplayTags();
			}
		}

		CurrentDialogue->ClearController();
		CurrentDialogue = nullptr;
	}

	ActiveJumpBack.Clear();
}

void ADialogueController::Skip() const
{
	if (CurrentDialogue)
	{
		CurrentDialogue->Skip();
	}
}

void ADialogueController::ClearNodeVisits()
{
	if (CurrentDialogue)
	{
		ClearAllNodeVisitsForDialogue(CurrentDialogue);
	}
}

void ADialogueController::SetSpeaker(FName InName,
	UDialogueSpeakerComponent* InSpeaker)
{
	if (CurrentDialogue && InSpeaker)
	{
		CurrentDialogue->SetSpeaker(InName, InSpeaker);
	}
}

FDialogueHistories ADialogueController::GetDialogueRecords() const
{
	return DialogueHistories;
}

void ADialogueController::ClearDialogueRecords()
{
	DialogueHistories.Histories.Empty();
}

void ADialogueController::ImportDialogueRecords(FDialogueHistories InRecords)
{
	DialogueHistories = InRecords;
}

bool ADialogueController::SpeakerInCurrentDialogue(UDialogueSpeakerComponent* TargetSpeaker) const
{
	//If no active dialogue, then automatically false
	if (!CurrentDialogue)
	{
		return false;
	}

	if (CurrentDialogue->bUseGenericSpeakerNames)
	{
		for (const auto& SpeakerKVP : CurrentDialogue->GetAllSpeakers())
		{
			if (SpeakerKVP.Value == TargetSpeaker)
				return true;
		}
		
		return false;
	}
	else
	{
		//Retrieve speakers
		TMap<FName, UDialogueSpeakerComponent*> Speakers = CurrentDialogue->GetAllSpeakers();
		TArray<FName> SpeakerRoles;
		Speakers.GetKeys(SpeakerRoles);

		//If any one speaker matches the target, true
		for (FName NextRole : SpeakerRoles)
		{
			if (Speakers[NextRole] == TargetSpeaker)
			{
				return true;
			}
		}	
	}

	//No speakers matched the target, false
	return false;
}

void ADialogueController::MarkNodeVisited(UDialogue* TargetDialogue, FName TargetNodeID)
{
	if (!TargetDialogue)
	{
		return;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();

	if (TargetDialogueName.IsEqual(NAME_None))
	{
		return;
	}

	//Create a new record if the target record does not exist
	if (!DialogueHistories.Histories.Contains(TargetDialogueName))
	{
		FDialogueHistory NewRecord;
		NewRecord.DialogueFName = TargetDialogueName;

		DialogueHistories.Histories.Add(TargetDialogueName, NewRecord);
	}

	//Mark the node visited in the record
	const auto& SpeakersIds = GetSpeakerIds(TargetDialogue);
	for (const auto& SpeakerId : SpeakersIds)
	{
		auto& CharacterDialogueHistory = DialogueHistories.Histories[TargetDialogueName].DialogueNodeHistory.FindOrAdd(SpeakerId);
		CharacterDialogueHistory.VisitedNodeIDs.Add(TargetNodeID);
	}
}

void ADialogueController::MarkNodeUnvisited(UDialogue* TargetDialogue, FName TargetNodeID)
{
	if (!TargetDialogue)
	{
		return;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();

	//If there is no record of that dialogue, do nothing
	if (!DialogueHistories.Histories.Contains(TargetDialogueName))
	{
		return;
	}

	//If there is a record, remove the target index from the visited nodes
	const auto& SpeakersIds = GetSpeakerIds(TargetDialogue);
	for (const auto& SpeakerId : SpeakersIds)
	{
		auto& CharacterDialogueHistory = DialogueHistories.Histories[TargetDialogueName].DialogueNodeHistory.FindOrAdd(SpeakerId);
		CharacterDialogueHistory.VisitedNodeIDs.Remove(TargetNodeID);
	}
}

void ADialogueController::ClearAllNodeVisitsForDialogue(UDialogue* TargetDialogue)
{
	if (!TargetDialogue)
	{
		return;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();

	//If there is no record of that dialogue, do nothing
	if (!DialogueHistories.Histories.Contains(TargetDialogueName))
	{
		return;
	}

	DialogueHistories.Histories[TargetDialogueName].DialogueNodeHistory.Empty();
}

bool ADialogueController::WasNodeVisited(const UDialogue* TargetDialogue, FName TargetNodeID) const
{
	if (!TargetDialogue)
	{
		return false;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();

	if (!DialogueHistories.Histories.Contains(TargetDialogueName))
	{
		return false;
	}

	const auto& SpeakersIds = GetSpeakerIds(TargetDialogue);
	for (const auto& SpeakerId : SpeakersIds)
	{
		auto CharacterDialogueHistory = DialogueHistories.Histories[TargetDialogueName].DialogueNodeHistory.Find(SpeakerId);
		if (CharacterDialogueHistory && CharacterDialogueHistory->VisitedNodeIDs.Contains(TargetNodeID))
			return true;
	}

	return false;
}

void ADialogueController::SetResumeNode(UDialogue* InDialogue, FName InNodeID)
{
	if (!InDialogue || InNodeID.IsNone())
	{
		return;
	}

	//Ensure there is a record
	FName RecordName = InDialogue->GetFName();
	if (!DialogueHistories.Histories.Contains(RecordName))
	{
		DialogueHistories.Histories.Add(RecordName);
	}

	//Set the record's resume node
	auto SpeakerIds = GetSpeakerIds(InDialogue);
	for (const auto& SpeakerId : SpeakerIds)
	{
		auto& CharacterHistory = DialogueHistories.Histories[RecordName].DialogueNodeHistory.FindOrAdd(SpeakerId);
		CharacterHistory.ResumeNodeID = InNodeID;
	}
}

TArray<FGuid> ADialogueController::GetSpeakerIds(const UDialogue* Dialogue) const
{
	const auto& Speakers = Dialogue->GetAllSpeakers();
	TArray<FGuid> SpeakerIds;
	SpeakerIds.Reserve(Speakers.Num());
	for (const auto& Speaker : Speakers)
		if (!Speaker.Value->IsPlayer())
			SpeakerIds.Add(Speaker.Value->GetDialogueSpeakerId());

	return SpeakerIds;
}

void ADialogueController::OpenDisplay_Implementation()
{
}

void ADialogueController::CloseDisplay_Implementation()
{
}

void ADialogueController::DisplaySpeech_Implementation(FSpeechDetails InSpeechDetails,
                                                       UDialogueSpeakerComponent* InSpeaker, int SpeechVariationIndex)
{
}

void ADialogueController::DisplayOptions_Implementation(const TArray<FSpeechDetails>& InOptions)
{
}

bool ADialogueController::CanOpenDisplay_Implementation() const
{
	return true;
}

void ADialogueController::HandleMissingSpeaker_Implementation(const FName& MissingName)
{
}

// G2VS2:
void ADialogueController::FDialogueJumpBack::Set(UDialogue* InDialogue, FName InNodeId)
{
	Dialogue = InDialogue;
	NodeId = InNodeId;
}

FName ADialogueController::FDialogueJumpBack::Get(UDialogue* InDialogue)
{
	if (Dialogue.IsNull())
		return NAME_None;
	
	FName Result = ensure(InDialogue == Dialogue) ? NodeId : NAME_None;
	Dialogue.Reset();
	NodeId = NAME_None;
	return Result;
}

void ADialogueController::FDialogueJumpBack::Clear()
{
	Dialogue.Reset();
	NodeId = NAME_None;
}

void ADialogueController::SetJumpBackNode(UDialogue* Dialogue, FName NodeId)
{
	ActiveJumpBack.Set(Dialogue, NodeId);
}

FName ADialogueController::GetJumpBackNode(UDialogue* Dialogue)
{
	return ActiveJumpBack.Get(Dialogue);
}

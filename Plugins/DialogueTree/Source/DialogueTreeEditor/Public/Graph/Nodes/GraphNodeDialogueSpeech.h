// Copyright Zachary Brett, 2024. All rights reserved.

#pragma once

//UE
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
//Plugin
#include "Graph/PickableDialogueSpeaker.h"
#include "SpeechDetails.h"
#include "GraphNodeDialogueEvent.h"
//Generated
#include "GraphNodeDialogueSpeech.generated.h"

class UDialogueSpeakerSocket;
class UDialogueTransition;

/**
 * Graph node representing an individual dialogue speech. 
 */

USTRUCT(BlueprintType)
struct FSpeechGesture
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FPickableDialogueSpeaker Speaker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag GestureTag_Obsolete;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="AI.Ability.Gesture,G2VS2.Character.Gesture"))
	FGameplayTagContainer GestureVariations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FSpeechGestureItemData> SpeechGestureItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0.f, ClampMin = 0.f, UIMax = 1.f, ClampMax = 1.f))
	float GestureChance = 0.8f;
};

USTRUCT(BlueprintType)
struct FSpeechVariationData
{
	GENERATED_BODY()

	/** The text/dialogue content of the speech */
	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FText SpeechText;

	/** The sound to play as audio for the speech */
	UPROPERTY(EditAnywhere)
	USoundCue* SpeechAudio;

};

UCLASS()
class DIALOGUETREEEDITOR_API UGraphNodeDialogueSpeech : public UGraphNodeDialogueEvent
{
	GENERATED_BODY()

public:
	/**
	* Pseudo-Constructor used to instantiate a new speech node template. Static.
	* 
	* @param Outer - UObject*, the owning object/graph for the node. 
	* @param InSpeaker - UDialogueSpeakerSocket*, the node's speaker. 
	* @param TransitionType - TSubclassOf<UDialogueTransition>, the type of 
	* transition for the node. 
	*/
	static UGraphNodeDialogueSpeech* MakeTemplate(UObject* Outer,
		UDialogueSpeakerSocket* InSpeaker, 
		TSubclassOf<UDialogueTransition> TransitionType);

public: 
	/** UObject Impl. */
	virtual void PostEditChangeProperty(
		struct FPropertyChangedEvent& PropertyChangedEvent) override;
	/** End UObject */

	/** UEdGraphNode Implementation */
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FName GetBaseID() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	/** End UEdGraphNode */

	/** UGraphNodeDialogueBase Implementation */
	virtual FText GetContextMenuName() const override;
	virtual EDialogueConnectionLimit GetOutputConnectionLimit() 
		const override;
	/** End UGraphNodeDialogueBase */

	/** UGraphNodeDialogue Implementation */
	virtual void CreateAssetNode(class UDialogue* InAsset) override;
	virtual bool CanCompileNode() override;
	virtual void LoadNodeData(UDialogueNode* InNode) override;
	/** End UGraphNodeDialogue */

public:
	/**
	* Retrieves the Speech Text. 
	* 
	* @return FText - Speech Text.
	*/
	FText GetSpeechText() const;

	/**
	* Retrieves the UClass type of the speech's assigned transition. 
	* 
	* @return UClass* - the speech's type. 
	*/
	UClass* GetTransitionType() const;

	/**
	* Sets the transition type to the provided subclass of UDialogueTransition.
	* 
	* @param InType - TSubclassOf<UDialogueTransition>, the new transition type.
	*/
	void SetTransitionType(TSubclassOf<UDialogueTransition> InType);

	/**
	* Sets the speech's speaker to the provided socket. 
	* 
	* @param InSpeaker - UDialogueSpeakerSocket*, the new speaker socket. 
	*/
	void SetSpeaker(UDialogueSpeakerSocket* InSpeaker);

	/**
	* Retrieves the speaker socket associated with this speech. 
	* 
	* @return UDialogueSpeakerSocket*, the speaker. 
	*/
	UDialogueSpeakerSocket* GetSpeaker() const;

private:
	/**
	* Behaviors for when the title of the speech changes. 
	*/
	void OnChangeSpeechTitle();

	/**
	* Behaviors for when the speech's transition type changes. 
	*/
	void OnChangeTransitionType();

private:
	/** The speaker associated with this speech */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	FPickableDialogueSpeaker Speaker;

	/** The name/ID of the speech */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	FName SpeechTitle;

	/** Whether the speech content should be ignored when entering the node.
		This is useful for cases when we want to ommit the speech from logs
		and such (CRPG type systems). */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	bool bIgnoreContent = false;

	// G2VS2 19.11.2024 @AK: FSpeechVariationData is added for G2VS2 purposes

	/** The text/dialogue content of the speech */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	TArray<FSpeechVariationData> SpeechVariations;

	// G2VS2 19.11.2024 @AK: commented FText and USoundBase* property as we need to support speech variations for a single dialogue node for cases like
	// greetings, where a character can say "hi, hello, hey can I talk to you", etc
	// UPROPERTY(EditAnywhere, Category = "SpeechContent", meta = (MultiLine = true))
	// FText SpeechText;
	//
	// /** The sound to play as audio for the speech */
	// UPROPERTY(EditAnywhere, Category = "SpeechContent")
	// USoundBase* SpeechAudio;

	/** The minimum time for the speech to play before transitioning (unless
	* skipped) */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	float MinimumPlayTime = 3.f;

	/** Whether the speech's content can be skipped or must be listened to */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	bool bCanSkip = true;

	/** GameplayTags associated with the speech. */
	UPROPERTY(EditAnywhere, Category = "BehaviorFlags")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleAnywhere, meta=(UIMin = 0.f, ClampMin = 0.f, UIMax = 1.f, ClampMax = 1.f), Category = "Gesture")
	float GesturePlayChance_Obsolete = 0.8f;
	
	UPROPERTY(VisibleAnywhere, meta=(EditCondition="GesturePlayChance > 0.f"), Category = "Gesture")
	FGameplayTag GestureToPlay_Obsolete;

	UPROPERTY(EditAnywhere, Category = "Gesture")
	TArray<FSpeechGesture> SpeechGestures;

	// You can put parameters in speech phrases. Before rendering an actual output,
	// each speech option is scanned for these parameters by maps key, which should be in curly brackets
	// and then IDialogueTreeGameMode is querried to return a text for the parameter, searched by gameplay tag
	// example. speech option text: Here, I managed to sell your goods for {Revenue} coins
	// and in SpeechParameters: SpeechParameters.Add("Revenue", "Quest.TradingQuest.Dialogue.Parameter.Revenue")
	// and IDialogueTreeGameMode::GetSpeechParameter("Quest.TradingQuest.Dialogue.Parameter.Revenue") returns FText of "134"
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	TMap<FString, FGameplayTag> SpeechParameters;

	UPROPERTY(EditAnywhere, Category = "Requirements")
	TMap<FGameplayTag, FDialogueOptionAttributeCheckContainer> AttributeChecks;
	
	/** The strategy used to transition out of the speech/continue dialogue */
	UPROPERTY(EditAnywhere, NoClear, Category = "Transition")
	TSubclassOf<UDialogueTransition> TransitionType;
};

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/Interface.h"

#include "DialogueSubsystem.generated.h"

class UFlowNode_DialogueLine;

USTRUCT(BlueprintType)
struct FDialogueLinePayload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
	FText Speaker;

	UPROPERTY(BlueprintReadOnly)
	FText Text;
};

USTRUCT(BlueprintType)
struct FDialogueChoicePayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FText> ChoiceTexts;

	// UI側で押されたIndex（元配列に一致させる）
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> SourceIndices;
};

// --- Events for UI ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueLineRequested, const FDialogueLinePayload&, Payload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceRequested, const FDialogueChoicePayload&, Payload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEvent);


UCLASS(DisplayName = "Dialogue Subsystem")
class OMEGA_API UDialogueSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

public:
	// ===== UI Events =====
	UPROPERTY(BlueprintAssignable, Category="Dialogue|Events")
	FOnDialogueLineRequested OnDialogueLineRequested;

	UPROPERTY(BlueprintAssignable, Category="Dialogue|Events")
	FOnDialogueChoiceRequested OnDialogueChoiceRequested;

	UPROPERTY(BlueprintAssignable, Category="Dialogue|Events")
	FOnDialogueEvent OnDialogueClosed;

	UPROPERTY(BlueprintAssignable, Category="Dialogue|Events")
	FOnDialogueEvent OnDialogueOpened;

	// ===== Requests from Flow Nodes =====

	/** FlowNode_DialogueLine から呼ばれる：台詞の表示要求 */
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void RequestLine(const FText& Speaker, const FText& Text, UObject* RequesterNode);

	/** FlowNode_DialogueChoice から呼ばれる：選択肢の表示要求 */
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void RequestChoice(const TArray<FText>& ChoiceTexts, const TArray<int32>& SourceIndices, UObject* RequesterNode);

	// ===== Responses from UI =====
    	/** 台詞を読み終えた（次へ） */
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void Continue();

	/** 選択肢を選んだ */
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void SelectChoice(int32 SourceIndex);

	/** UIを閉じる（会話強制終了など） */
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void Close();

	// ===== Debug / State =====
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	bool IsInDialogue() const { return bInDialogue; }

private:
	UPROPERTY()
	TWeakObjectPtr<UObject> ActiveRequesterNode;

	UPROPERTY()
	bool bInDialogue = false;

	FTimerHandle CloseTimer;
};

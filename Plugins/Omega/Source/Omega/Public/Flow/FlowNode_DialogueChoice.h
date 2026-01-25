#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_DialogueChoice.generated.h"

USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Text;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsAvailable{false};
};



UCLASS(DisplayName = "Dialogue Choice")
class OMEGA_API UFlowNode_DialogueChoice : public UFlowNode
{
    GENERATED_BODY()

public:

    UFlowNode_DialogueChoice();

	/** 選択肢リスト */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueChoice> Choices;

	/** UI から選択結果を受け取る */
	UFUNCTION()
	void SelectChoice(int32 ChoiceIndex);
    
protected:

    virtual void ExecuteInput(const FName& PinName) override;

private:
    void CreateOutputPins();
    
};

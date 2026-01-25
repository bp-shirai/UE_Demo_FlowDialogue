#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_DialogueLine.generated.h"

UCLASS(DisplayName = "Dialogue Line")
class OMEGA_API UFlowNode_DialogueLine : public UFlowNode
{
    GENERATED_BODY()

public:

    UFlowNode_DialogueLine();

    UPROPERTY(EditAnywhere, Category = "Dialogue")
    FText SpeakerName;

    UPROPERTY(EditAnywhere, Category = "Dialogue", meta=(MultiLine=true))
    FText DialogueText;


    virtual void ExecuteInput(const FName& PinName) override;

    UFUNCTION()
    void OnDialogueFinished();
    
};

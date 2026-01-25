#include "Flow/FlowNode_DialogueLine.h"

#include "FlowSubsystem.h"

#include "Flow/DialogueSubsystem.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_DialogueLine)

UFlowNode_DialogueLine::UFlowNode_DialogueLine()
{
    InputPins.Empty();
    InputPins.Add(FFlowPin(TEXT("In")));
    OutputPins.Empty();
    OutputPins.Add(FFlowPin(TEXT("Finish")));
#if WITH_EDITOR
    Category = TEXT("Flow Dialogue");
#endif
}

void UFlowNode_DialogueLine::ExecuteInput(const FName& PinName)
{
    if (UDialogueSubsystem* Dialogue = GetWorld()->GetSubsystem<UDialogueSubsystem>())
    {
        Dialogue->RequestLine(SpeakerName, DialogueText, this);
    }
}



void UFlowNode_DialogueLine::OnDialogueFinished()
{
    TriggerOutput(TEXT("Finish"));
}

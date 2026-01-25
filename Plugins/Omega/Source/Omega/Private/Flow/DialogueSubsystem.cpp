#include "Flow/DialogueSubsystem.h"

#include "Flow/FlowNode_DialogueLine.h"
#include "Flow/FlowNode_DialogueChoice.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueSubsystem)

void UDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UDialogueSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UDialogueSubsystem::RequestLine(const FText& Speaker, const FText& Text, UObject* RequesterNode)
{
    CloseTimer.Invalidate();

    if (bInDialogue == false)
    {
        OnDialogueOpened.Broadcast();
    }

    bInDialogue         = true;
    ActiveRequesterNode = RequesterNode;

    FDialogueLinePayload Payload;
    Payload.Speaker = Speaker;
    Payload.Text    = Text;

    OnDialogueLineRequested.Broadcast(Payload);
}

void UDialogueSubsystem::RequestChoice(const TArray<FText>& ChoiceTexts, const TArray<int32>& SourceIndices, UObject* RequesterNode)
{
    bInDialogue         = true;
    ActiveRequesterNode = RequesterNode;

    FDialogueChoicePayload Payload;
    Payload.ChoiceTexts   = ChoiceTexts;
    Payload.SourceIndices = SourceIndices;

    OnDialogueChoiceRequested.Broadcast(Payload);
}

void UDialogueSubsystem::Continue()
{
    // 台詞ノードの場合だけ継続
    if (!ActiveRequesterNode.IsValid())
    {
        return;
    }

    if (UFlowNode_DialogueLine* LineNode = Cast<UFlowNode_DialogueLine>(ActiveRequesterNode.Get()))
    {
        LineNode->OnDialogueFinished();
        ActiveRequesterNode = nullptr;

        GetWorld()->GetTimerManager().SetTimer(CloseTimer, this, &ThisClass::Close, 0.2f, false);
        return;
    }
}

void UDialogueSubsystem::SelectChoice(int32 SourceIndex)
{
    if (!ActiveRequesterNode.IsValid())
    {
        return;
    }

    if (UFlowNode_DialogueChoice* ChoiceNode = Cast<UFlowNode_DialogueChoice>(ActiveRequesterNode.Get()))
    {
        ChoiceNode->SelectChoice(SourceIndex);
        ActiveRequesterNode = nullptr;
        return;
    }
}

void UDialogueSubsystem::Close()
{
    bInDialogue         = false;
    ActiveRequesterNode = nullptr;

    OnDialogueClosed.Broadcast();
}

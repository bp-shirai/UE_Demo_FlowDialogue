#include "Flow/FlowNode_DialogueChoice.h"

#include "FlowSubsystem.h"

#include "Flow/DialogueSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_DialogueChoice)

UFlowNode_DialogueChoice::UFlowNode_DialogueChoice()
{
    InputPins.Empty();
    InputPins.Add(FFlowPin(TEXT("In")));

#if WITH_EDITOR
    Category = TEXT("Flow Dialogue");
#endif
}

void UFlowNode_DialogueChoice::ExecuteInput(const FName& PinName)
{
    // 出力ピン生成（エディタ変更対応）
    CreateOutputPins();

    TArray<FText> Texts;
    TArray<int32> Indices;

    for (int32 i = 0; i < Choices.Num(); ++i)
    {
        // ここに Condition 判定を入れてもいい
        Texts.Add(Choices[i].Text);
        Indices.Add(i); // 元Index
    }

    // UI に選択肢を投げる
    if (UDialogueSubsystem* Dialogue = GetWorld()->GetSubsystem<UDialogueSubsystem>())
    {
        Dialogue->RequestChoice(Texts, Indices, this);
    }
}

void UFlowNode_DialogueChoice::SelectChoice(int32 ChoiceIndex)
{
    if (!Choices.IsValidIndex(ChoiceIndex))
    {
        return;
    }

    const FName OutputPin = FName(*FString::Printf(TEXT("Choice_%d"), ChoiceIndex + 1));

    TriggerOutput(OutputPin);
}

void UFlowNode_DialogueChoice::CreateOutputPins()
{
    OutputPins.Empty();

    for (int i = 0; i < Choices.Num(); i++)
    {
        OutputPins.Add(*FString::Printf(TEXT("Choice_%d"), i + 1));
    }
}

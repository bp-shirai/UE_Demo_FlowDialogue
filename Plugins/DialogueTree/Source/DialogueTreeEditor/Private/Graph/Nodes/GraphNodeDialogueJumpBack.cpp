// 


#include "Graph/Nodes/GraphNodeDialogueJumpBack.h"
#include "Dialogue.h"
#include "Nodes/DialogueJumpNode.h"
#include "Nodes/DialogueJumpBackNode.h"

#define LOCTEXT_NAMESPACE "GraphNodeDialogueJumpBack"

UGraphNodeDialogueJumpBack* UGraphNodeDialogueJumpBack::MakeTemplate(UObject* Outer)
{
    return NewObject<UGraphNodeDialogueJumpBack>(Outer);
}

FText UGraphNodeDialogueJumpBack::GetTooltipText() const
{
    return LOCTEXT("TooltipText",  "Reverts the flow of dialogue to a specified node.");
}

void UGraphNodeDialogueJumpBack::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, "MultipleNodes", FName());
    CreatePin(EGPD_Output, "MultipleNodes", FName());
}

void UGraphNodeDialogueJumpBack::CreateAssetNode(UDialogue* InAsset)
{
    UDialogueJumpBackNode* NewNode = NewObject<UDialogueJumpBackNode>(InAsset);
    check(NewNode);
    SetAssetNode(NewNode);
}

FName UGraphNodeDialogueJumpBack::GetBaseID() const
{
    return "Jump back";
}

#undef LOCTEXT_NAMESPACE
// 


#include "Graph/Nodes/GraphNodeDialogueSetJumpBack.h"

#include "Dialogue.h"
#include "DialogueNodeSocket.h"
#include "Graph/DialogueEdGraph.h"
#include "Nodes/DialogueJumpNode.h"
#include "LogDialogueTree.h"
#include "Nodes/DialogueSetJumpBackNode.h"

#define LOCTEXT_NAMESPACE "GraphNodeDialogueSetJumpBack"

UGraphNodeDialogueSetJumpBack* UGraphNodeDialogueSetJumpBack::MakeTemplate(UObject* Outer)
{
    return NewObject<UGraphNodeDialogueSetJumpBack>(Outer);
}

void UGraphNodeDialogueSetJumpBack::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateDialogueNode();
}

FText UGraphNodeDialogueSetJumpBack::GetTooltipText() const
{
    return LOCTEXT("TooltipText", "Sets target for jump back.");
}

void UGraphNodeDialogueSetJumpBack::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, "MultipleNodes", FName());
    CreatePin(EGPD_Output, "MultipleNodes", FName());
}

void UGraphNodeDialogueSetJumpBack::PostPasteNode()
{
    Super::PostPasteNode();

    check(GetDialogueGraph());
    UDialogue* DialogueAsset = GetDialogueGraph()->GetDialogue();
    check(DialogueAsset);

    JumpTarget = DuplicateObject(JumpTarget, DialogueAsset);
}

void UGraphNodeDialogueSetJumpBack::CreateAssetNode(UDialogue* InAsset)
{
    UDialogueSetJumpBackNode* NewNode = NewObject<UDialogueSetJumpBackNode>(InAsset);
    check(NewNode);

    SetAssetNode(NewNode);
}

void UGraphNodeDialogueSetJumpBack::FinalizeAssetNode()
{
    UDialogueSetJumpBackNode* TargetAssetNode = CastChecked<UDialogueSetJumpBackNode>(GetAssetNode());
    UGraphNodeDialogue* TargetGraphNode = GetJumpTarget();

    if (!TargetGraphNode)
    {
        UE_LOG(LogDialogueTree, Warning, TEXT("Attempting to compile ""set jump back"" node with no target node set"));
        return;
    }
    
    if (!TargetGraphNode->GetAssetNode())
        return;

    TargetAssetNode->SetJumpTarget(TargetGraphNode->GetAssetNode());
}

bool UGraphNodeDialogueSetJumpBack::CanCompileNode()
{
    UDialogueEdGraph* Graph = GetDialogueGraph();
    check(Graph);

    UGraphNodeDialogue* TargetNode = GetJumpTarget();

    if (TargetNode && Graph->ContainsNode(TargetNode->GetID()) && TargetNode != this)
    {
        SetErrorFlag(false);
        return true;
    }

    SetErrorFlag(true);
    return false;
}

FName UGraphNodeDialogueSetJumpBack::GetBaseID() const
{
    return "Set jump back";
}

void UGraphNodeDialogueSetJumpBack::RegenerateNodeConnections(UDialogueEdGraph* DialogueGraph)
{
    Super::RegenerateNodeConnections(DialogueGraph);

    if (!DialogueGraph)
        return;
    
    UDialogueSetJumpBackNode* JumpNode = Cast<UDialogueSetJumpBackNode>(GetAssetNode());
    if (!JumpNode)
        return;
    
    if (!JumpNode->GetJumpTarget())
        return;

    JumpTarget = NewObject<UDialogueNodeSocket>(this);
    JumpTarget->SetDialogueNode(JumpNode->GetJumpTarget());
    JumpTarget->SetGraphNode(DialogueGraph->GetNode(JumpNode->GetJumpTarget()->GetNodeID()));
}

UGraphNodeDialogue* UGraphNodeDialogueSetJumpBack::GetJumpTarget()
{
    if (!JumpTarget || !JumpTarget->GetGraphNode())
    {
        return nullptr;
    }

    return Cast<UGraphNodeDialogue>(JumpTarget->GetGraphNode());
}

const TArray<UGraphNodeDialogueBase*> UGraphNodeDialogueSetJumpBack::GetDirectChildren() const
{
    TArray<UGraphNodeDialogueBase*> Result = Super::GetDirectChildren();
    if (JumpTarget != nullptr)
        if (auto DialogueNode = Cast<UGraphNodeDialogueBase>( JumpTarget->GetGraphNode()); ensure(DialogueNode))
            Result.Add(DialogueNode);

    return Result;
}

#undef LOCTEXT_NAMESPACE

// 


#include "Nodes/DialogueSetJumpBackNode.h"

#include "Dialogue.h"

void UDialogueSetJumpBackNode::EnterNode()
{
	Super::EnterNode();
	Dialogue->SetJumpBackNode(JumpTarget);
	if (!Children.IsEmpty())
		Dialogue->TraverseNode(Children[0]);
	else
		Dialogue->EndDialogue();
}

FDialogueOption UDialogueSetJumpBackNode::GetAsOption()
{
	if (JumpTarget)
	{
		FSpeechDetails OptionDetails = JumpTarget->GetAsOption().Details;
		return FDialogueOption{ OptionDetails, this };
	}

	return FDialogueOption();
}

void UDialogueSetJumpBackNode::SetJumpTarget(UDialogueNode* InTarget)
{
	check(InTarget);
	JumpTarget = InTarget;
}

UDialogueNode* UDialogueSetJumpBackNode::GetJumpTarget() const
{
	return JumpTarget;
}

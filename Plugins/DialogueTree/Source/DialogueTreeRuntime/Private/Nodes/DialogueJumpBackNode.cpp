// 


#include "Nodes/DialogueJumpBackNode.h"

#include "Dialogue.h"

void UDialogueJumpBackNode::EnterNode()
{
	Super::EnterNode();
	bool bJumped = Dialogue->JumpBack();
	if (bJumped)
		return;
	
	if (!Children.IsEmpty())
	{
		Dialogue->TraverseNode(Children[0]);
	}
	else
	{
		Dialogue->EndDialogue();
	}		
}

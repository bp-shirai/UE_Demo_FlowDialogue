// 

#pragma once

#include "CoreMinimal.h"
#include "DialogueNode.h"
#include "DialogueSetJumpBackNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUETREERUNTIME_API UDialogueSetJumpBackNode : public UDialogueNode
{
	GENERATED_BODY()

public:
	/** UDialogueNode Implementation */
	virtual void EnterNode() override;
	virtual FDialogueOption GetAsOption() override;
	/** End UDialogueNode */

	/**
	* Sets the target of the node's jump. 
	* 
	* @param InTarget - UDialogueNode*, the node to jump to. 
	*/
	void SetJumpTarget(UDialogueNode* InTarget);

	/**
	* Gets the target of the node's jump. 
	* 
	* @return UDialogueNode*, the target. 
	*/
	UDialogueNode* GetJumpTarget() const;

private:
	/** The target node to "jump" to */
	UPROPERTY()
	TObjectPtr<UDialogueNode> JumpTarget;
};

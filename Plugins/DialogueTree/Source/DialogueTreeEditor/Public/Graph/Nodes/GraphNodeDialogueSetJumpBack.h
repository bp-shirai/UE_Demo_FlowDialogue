// 

#pragma once

#include "CoreMinimal.h"
#include "GraphNodeDialogue.h"
#include "GraphNodeDialogueSetJumpBack.generated.h"

class UDialogueNodeSocket;
/**
 * 
 */
UCLASS()
class DIALOGUETREEEDITOR_API UGraphNodeDialogueSetJumpBack : public UGraphNodeDialogue
{
	GENERATED_BODY()

public:
	/** Pseudo constructor for creating a blank template jump node. Static.
	*
	* @param Outer - UObject*, the owner for the new template.
	*/
	static UGraphNodeDialogueSetJumpBack* MakeTemplate(UObject* Outer);

public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	/** UEdGraphNode Implementation */
	virtual FText GetTooltipText() const override;
	virtual void AllocateDefaultPins() override;
	virtual void PostPasteNode() override;
	/** End UEdGraphNode */

	/** UGraphNodeDialogue Impl. */
	virtual void CreateAssetNode(class UDialogue* InAsset) override;
	virtual void FinalizeAssetNode() override;
	virtual bool CanCompileNode() override;
	virtual FName GetBaseID() const override;
	virtual void RegenerateNodeConnections(UDialogueEdGraph* DialogueGraph) override;
	/** End UGraphNodeDialogue */
	
	/**
	* Retrieves the jump node's target Dialogue Node.  
	* 
	* @return UGraphNodeDialogue*, the jump target node. Nullptr if none set.
	*/
	UGraphNodeDialogue* GetJumpTarget();

	virtual const TArray<UGraphNodeDialogueBase*> GetDirectChildren() const override;
	
private:
	/** The node to jump to */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TObjectPtr<UDialogueNodeSocket> JumpTarget;
};

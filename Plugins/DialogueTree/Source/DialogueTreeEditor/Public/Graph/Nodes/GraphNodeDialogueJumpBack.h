// 

#pragma once

#include "CoreMinimal.h"
#include "GraphNodeDialogue.h"
#include "GraphNodeDialogueJumpBack.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUETREEEDITOR_API UGraphNodeDialogueJumpBack : public UGraphNodeDialogue
{
	GENERATED_BODY()

public:
	/** Pseudo constructor for creating a blank template jump node. Static.
	*
	* @param Outer - UObject*, the owner for the new template.
	*/
	static UGraphNodeDialogueJumpBack* MakeTemplate(UObject* Outer);

public:
	/** UEdGraphNode Implementation */
	virtual FText GetTooltipText() const override;
	virtual void AllocateDefaultPins() override;
	/** End UEdGraphNode */

	/** UGraphNodeDialogue Impl. */
	virtual void CreateAssetNode(class UDialogue* InAsset) override;
	virtual FName GetBaseID() const override;
	/** End UGraphNodeDialogue */
};

#pragma once

#include "SGraphNodeDialogueBase.h"

class UGraphNodeDialogueSetJumpBack;
class UGraphNodeDialogueJumpBack;

class DIALOGUETREEEDITOR_API SGraphNodeDialogueSetJumpBack : public SGraphNodeDialogueBase
{
public:
	//Slate Arguments 
	SLATE_BEGIN_ARGS(SGraphNodeDialogueSetJumpBack) {}
	SLATE_END_ARGS()

	/** Slate Constructor */
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

protected:
	/** SGraphNode Implementation */
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	/** End SGraphNode */

private:
	/**
	* Retrieves the display text for indicating the node's jump target. 
	* 
	* @return FText - the display text. 
	*/
	FText GetDisplayText() const; 

private:
	/** Cached Jump Node */
	TObjectPtr<UGraphNodeDialogueSetJumpBack> JumpNode;

	/** Constants */
	float CONTENT_PADDING = 10.f;
};

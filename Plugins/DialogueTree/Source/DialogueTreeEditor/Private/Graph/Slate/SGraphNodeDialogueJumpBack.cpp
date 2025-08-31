#include "Graph/Slate/SGraphNodeDialogueJumpBack.h"

#include "Graph/Nodes/GraphNodeDialogueJumpBack.h"

#define LOCTEXT_NAMESPACE "SGraphNodeDialogueJumpBack"

void SGraphNodeDialogueJumpBack::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	/** Cache the downcast jump node */
	JumpNode = Cast<UGraphNodeDialogueJumpBack>(InNode);
	check(JumpNode);
	
	SGraphNodeDialogueBase::Construct(SGraphNodeDialogueBase::FArguments(), InNode);
}

TSharedRef<SWidget> SGraphNodeDialogueJumpBack::CreateNodeContentArea()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(CONTENT_PADDING)
		[
			SNew(STextBlock)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", BASE_FONT_SIZE))
			.Justification(TEXT_JUSTIFY)
			.Text(GetDisplayText())
			.WrapTextAt(WRAP_TEXT_AT)
		];
}

FText SGraphNodeDialogueJumpBack::GetDisplayText() const
{ 
	return LOCTEXT("JumpBackNodeText", "Jump back");
}

#undef LOCTEXT_NAMESPACE

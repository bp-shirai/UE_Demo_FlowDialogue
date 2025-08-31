#include "Graph/Slate/SGraphNodeDialogueSetJumpBack.h"

#include "Graph/Nodes/GraphNodeDialogueSetJumpBack.h"

#define LOCTEXT_NAMESPACE "SGraphNodeDialogueSetJumpBack"

void SGraphNodeDialogueSetJumpBack::Construct(const FArguments& InArgs,  UEdGraphNode* InNode)
{
	/** Cache the downcast jump node */
	JumpNode = Cast<UGraphNodeDialogueSetJumpBack>(InNode);
	check(JumpNode);
	
	SGraphNodeDialogueBase::Construct(SGraphNodeDialogueBase::FArguments(), InNode);
}

TSharedRef<SWidget> SGraphNodeDialogueSetJumpBack::CreateNodeContentArea()
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

FText SGraphNodeDialogueSetJumpBack::GetDisplayText() const
{ 
	check(JumpNode);
	if (JumpNode->GetJumpTarget() == nullptr)
	{
		return LOCTEXT("DefaultDisplayText",  "Set Jump Target Not Set");
	}
	else
	{
		FText BaseText = LOCTEXT("BaseText", "Set jump back to {0}");
		FText TargetName = FText::FromName(JumpNode->GetJumpTarget()->GetID());
		return FText::Format(BaseText, TargetName);
	}
}

#undef LOCTEXT_NAMESPACE


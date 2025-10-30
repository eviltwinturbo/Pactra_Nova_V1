#include "SCaseAgentWindow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SVerticalBox.h"

void SCaseAgentWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Case Agent - Day 0 Stub")))
		]
	];
}

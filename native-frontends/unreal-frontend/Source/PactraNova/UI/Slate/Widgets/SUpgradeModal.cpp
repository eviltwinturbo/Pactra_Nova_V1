#include "SUpgradeModal.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SVerticalBox.h"
#include "Widgets/Layout/SHorizontalBox.h"

void SUpgradeModal::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Upgrade Your Tier")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("This feature requires a higher tier subscription.")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SHorizontalBox)
			// TODO: Add upgrade button and cancel button
		]
	];
}

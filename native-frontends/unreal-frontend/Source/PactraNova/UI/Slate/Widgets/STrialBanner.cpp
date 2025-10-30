#include "STrialBanner.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SHorizontalBox.h"

void STrialBanner::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("You are on a trial tier. Upgrade to access all features.")))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10)
		[
			// TODO: Add upgrade link button
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Upgrade")))
		]
	];
}

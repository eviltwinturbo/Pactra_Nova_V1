#include "SSettingsPane.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SVerticalBox.h"

void SSettingsPane::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Settings")))
		]
	];

	// TODO: Add settings options (notifications, privacy, appearance, etc.)
}

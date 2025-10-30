#include "SOrbitSidebar.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SVerticalBox.h"

void SOrbitSidebar::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Orbit Sidebar")))
		]
	];

	// TODO: Add channel list, direct message list, and navigation buttons
}

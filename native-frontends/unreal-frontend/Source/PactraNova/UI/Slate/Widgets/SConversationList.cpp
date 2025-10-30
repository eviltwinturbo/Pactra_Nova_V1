#include "SConversationList.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SVerticalBox.h"

void SConversationList::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Conversations")))
		]
	];

	// TODO: Populate with conversation list items fetched from API
}

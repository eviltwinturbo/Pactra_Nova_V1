#include "SOrbitWindow.h"
#include "../Widgets/SOrbitSidebar.h"
#include "../Widgets/SConversationList.h"
#include "../Widgets/SMessagePane.h"
#include "../Widgets/SSettingsPane.h"
#include "../Widgets/STrialBanner.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SHorizontalBox.h"
#include "Widgets/Layout/SVerticalBox.h"

void SOrbitWindow::Construct(const FArguments& InArgs)
{
	// TODO: Initialize child widgets with proper layout
	SidebarWidget = SNew(SOrbitSidebar);
	ConversationListWidget = SNew(SConversationList);
	MessagePaneWidget = SNew(SMessagePane);
	SettingsPaneWidget = SNew(SSettingsPane);
	TrialBannerWidget = SNew(STrialBanner);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(TrialBannerWidget, STrialBanner)
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(SidebarWidget, SOrbitSidebar)
			]
			+ SHorizontalBox::Slot()
			.FillWidth(0.25f)
			[
				SAssignNew(ConversationListWidget, SConversationList)
			]
			+ SHorizontalBox::Slot()
			.FillWidth(0.75f)
			[
				SAssignNew(MessagePaneWidget, SMessagePane)
			]
		]
	];
}

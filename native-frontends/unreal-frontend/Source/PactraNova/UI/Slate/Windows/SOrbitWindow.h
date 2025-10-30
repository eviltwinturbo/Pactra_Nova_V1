#pragma once

#include "Widgets/SCompoundWidget.h"

class SOrbitSidebar;
class SConversationList;
class SMessagePane;
class SSettingsPane;
class STrialBanner;

/**
 * SOrbitWindow - Main window for Orbit Messenger UI
 * Manages layout of sidebar, conversation list, message pane, and settings
 */
class SOrbitWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOrbitWindow)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// Child widget references
	TSharedPtr<SOrbitSidebar> SidebarWidget;
	TSharedPtr<SConversationList> ConversationListWidget;
	TSharedPtr<SMessagePane> MessagePaneWidget;
	TSharedPtr<SSettingsPane> SettingsPaneWidget;
	TSharedPtr<STrialBanner> TrialBannerWidget;
};

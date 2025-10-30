#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * SOrbitSidebar - Sidebar navigation widget for Orbit Messenger
 * Displays channels, direct messages, and navigation options
 */
class SOrbitSidebar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOrbitSidebar)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};

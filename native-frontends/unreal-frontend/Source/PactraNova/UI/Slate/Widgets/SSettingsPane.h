#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * SSettingsPane - Settings and configuration interface
 */
class SSettingsPane : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSettingsPane)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};

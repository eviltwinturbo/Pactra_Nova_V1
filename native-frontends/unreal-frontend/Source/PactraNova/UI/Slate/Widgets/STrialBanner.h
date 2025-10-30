#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * STrialBanner - Banner displayed for trial tier users
 * Shows trial period remaining and upgrade prompt
 */
class STrialBanner : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STrialBanner)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};

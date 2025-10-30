#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * SUpgradeModal - Modal dialog for tier upgrade prompts
 * Shown when user lacks tier permissions for features
 */
class SUpgradeModal : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUpgradeModal)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};

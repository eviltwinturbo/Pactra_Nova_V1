#pragma once

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleMacros.h"

/**
 * FPactraNovaStyle - Slate UI styling for Pactra Nova application
 */
class FPactraNovaStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static TSharedPtr<class FSlateStyleSet> Get();

private:
	static TSharedPtr<class FSlateStyleSet> StyleSet;
};

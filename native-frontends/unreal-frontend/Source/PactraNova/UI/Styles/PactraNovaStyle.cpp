#include "PactraNovaStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"

TSharedPtr<FSlateStyleSet> FPactraNovaStyle::StyleSet = nullptr;

void FPactraNovaStyle::Initialize()
{
	if (!StyleSet.IsValid())
	{
		StyleSet = MakeShareable(new FSlateStyleSet("PactraNovaStyle"));
		
		// TODO: Define colors, fonts, sizes for UI elements
		// Define spacing, padding, margins
		// Define button styles, text styles, panel backgrounds
		
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

void FPactraNovaStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
		StyleSet.Reset();
	}
}

TSharedPtr<FSlateStyleSet> FPactraNovaStyle::Get()
{
	return StyleSet;
}

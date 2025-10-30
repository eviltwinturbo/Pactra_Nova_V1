#include "SProvenanceOverlay.h"
#include "Widgets/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Styling/AppStyle.h"

void SProvenanceOverlay::Construct(const FArguments& InArgs)
{
	FullHash = InArgs._Hash;
	AbbreviatedHash = GetAbbreviatedHash(FullHash);

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(200.0f)
		.HeightOverride(40.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.OnMouseButtonDown(this, &SProvenanceOverlay::OnCopyToClipboard)
			[
				SAssignNew(HashDisplay, STextBlock)
				.Text(FText::FromString(AbbreviatedHash))
				.Font(FAppStyle::GetFontStyle("SmallFont"))
				.ColorAndOpacity(this, &SProvenanceOverlay::GetHashColor)
				.ToolTipText(FText::FromString(FullHash))
			]
		]
	];
}

SProvenanceOverlay::~SProvenanceOverlay()
{
}

void SProvenanceOverlay::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bIsHovering = true;
	if (HashDisplay.IsValid())
	{
		HashDisplay->SetText(FText::FromString(FullHash));
	}
}

void SProvenanceOverlay::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bIsHovering = false;
	if (HashDisplay.IsValid())
	{
		HashDisplay->SetText(FText::FromString(AbbreviatedHash));
	}
}

FReply SProvenanceOverlay::OnCopyToClipboard()
{
	FPlatformApplicationMisc::ClipboardCopy(*FullHash);
	return FReply::Handled();
}

FText SProvenanceOverlay::GetDisplayText() const
{
	return FText::FromString(bIsHovering ? FullHash : AbbreviatedHash);
}

FString SProvenanceOverlay::GetAbbreviatedHash(const FString& InHash) const
{
	if (InHash.Len() > 16)
	{
		return InHash.Left(8) + TEXT("...") + InHash.Right(8);
	}
	return InHash;
}

FSlateColor SProvenanceOverlay::GetHashColor() const
{
	// Shimmer effect: cyan to lighter cyan
	return bIsHovering ? FLinearColor(0.0f, 1.0f, 1.0f, 1.0f) : FLinearColor(0.0f, 0.8f, 0.9f, 0.8f);
}

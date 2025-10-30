#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

/**
 * SProvenanceOverlay
 *
 * Reusable Slate widget for displaying and managing SHA256 provenance hashes.
 * Features:
 * - Displays hash on hover (abbreviated or full)
 * - Copy-to-clipboard functionality
 * - Tooltip with full hash
 * - Visual feedback (shimmer, glow)
 */
class SProvenanceOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SProvenanceOverlay)
		: _Hash("")
		, _DisplayMode(0) // 0=abbreviated, 1=full
		, _ShowOnHover(true)
		{}
		SLATE_ARGUMENT(FString, Hash)
		SLATE_ARGUMENT(int32, DisplayMode)
		SLATE_ARGUMENT(bool, ShowOnHover)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SProvenanceOverlay();

private:
	// Hash data
	FString FullHash;
	FString AbbreviatedHash; // First 8 + "..." + Last 8

	// UI state
	bool bIsHovering = false;
	TSharedPtr<STextBlock> HashDisplay;

	// Event handlers
	void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void OnMouseLeave(const FPointerEvent& MouseEvent);

	// Actions
	FReply OnCopyToClipboard();

	// Formatting
	FText GetDisplayText() const;
	FString GetAbbreviatedHash(const FString& InHash) const;
	FSlateColor GetHashColor() const;
};

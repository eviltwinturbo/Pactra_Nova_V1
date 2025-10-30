#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

class FUnionPulseStore;

/**
 * SUnionPulse
 * 
 * Header widget displaying real-time metrics: presence, bills, contracts, tokens.
 * Subscribes to FUnionPulseStore for reactive updates.
 * Visible in all authenticated screens.
 */
class SUnionPulse : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUnionPulse) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SUnionPulse();

private:
	// Event handlers
	void OnPresenceUpdated();
	void OnBillsUpdated();
	void OnContractsUpdated();
	void OnTokensUpdated();
	void OnTierChanged();

	// Widget update methods
	FText GetTierText() const;
	FText GetPresenceText() const;
	FText GetBillsText() const;
	FText GetContractsText() const;
	FText GetTokensText() const;

	FSlateColor GetTierBadgeColor() const;

	// Refs to key widgets
	TSharedPtr<STextBlock> TierBadge;
	TSharedPtr<STextBlock> PresenceLabel;
	TSharedPtr<STextBlock> BillsLabel;
	TSharedPtr<STextBlock> ContractsLabel;
	TSharedPtr<STextBlock> TokensLabel;

	// Store ref
	FUnionPulseStore* Store;

	// Delegate handles
	FDelegateHandle OnPresenceHandle;
	FDelegateHandle OnBillsHandle;
	FDelegateHandle OnContractsHandle;
	FDelegateHandle OnTokensHandle;
	FDelegateHandle OnTierHandle;
};

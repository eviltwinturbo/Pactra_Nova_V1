#pragma once

#include "CoreMinimal.h"

// Feature IDs for gating
enum class EGatedFeature : uint8
{
	Threads,
	Reactions,
	ReadReceipts,
	ContractComparison,
	ReportsCharts,
	Branding,
	Copilot,
	MAX_VALUE
};

/**
 * FTierGate
 * 
 * Centralized tier gating service.
 * Determines which features are available for current tier.
 * Extracts required tier from 403 responses.
 */
class FTierGate
{
public:
	// Check if feature is accessible for given tier
	static bool IsFeatureAccessible(EGatedFeature Feature, const FString& CurrentTier);

	// Extract required tier from 403 error metadata
	static FString ExtractRequiredTierFrom403(const FString& ErrorMeta);

	// Get feature name (for UI)
	static FString GetFeatureName(EGatedFeature Feature);

	// Get human-readable tier label
	static FString GetTierDisplayName(const FString& Tier);

	// Get feature unlock tier
	static FString GetFeatureUnlockTier(EGatedFeature Feature);

private:
	FTierGate() = delete;
};

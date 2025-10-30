#include "FTierGate.h"

bool FTierGate::IsFeatureAccessible(EGatedFeature Feature, const FString& CurrentTier)
{
	// Trial tier: only basic features
	if (CurrentTier == TEXT("Trial"))
	{
		return Feature == EGatedFeature::Threads ? false :
		       Feature == EGatedFeature::Reactions ? false :
		       Feature == EGatedFeature::ReadReceipts ? false :
		       Feature == EGatedFeature::ContractComparison ? false :
		       Feature == EGatedFeature::ReportsCharts ? false :
		       Feature == EGatedFeature::Branding ? false :
		       Feature == EGatedFeature::Copilot ? false :
		       true;
	}

	// Starter tier: add threads, reactions, read receipts, reports
	if (CurrentTier == TEXT("Starter"))
	{
		return Feature == EGatedFeature::ContractComparison ? false :
		       Feature == EGatedFeature::Branding ? false :
		       Feature == EGatedFeature::Copilot ? false :
		       true;
	}

	// Pro tier: add contract comparison, branding
	if (CurrentTier == TEXT("Pro"))
	{
		return Feature == EGatedFeature::Copilot ? false :
		       true;
	}

	// Enterprise tier: all features
	if (CurrentTier == TEXT("Enterprise"))
	{
		return true;
	}

	return false;
}

FString FTierGate::ExtractRequiredTierFrom403(const FString& ErrorMeta)
{
	// Expected format: "requiredTier:Pro" or similar
	if (ErrorMeta.Contains(TEXT("requiredTier:")))
	{
		FString Right = ErrorMeta.RightChop(ErrorMeta.Find(TEXT("requiredTier:")) + 13);
		return Right.Split(TEXT(","), &Right, nullptr) ? Right : Right;
	}
	return TEXT("Pro"); // Default fallback
}

FString FTierGate::GetFeatureName(EGatedFeature Feature)
{
	switch (Feature)
	{
		case EGatedFeature::Threads:
			return TEXT("Message Threads");
		case EGatedFeature::Reactions:
			return TEXT("Emoji Reactions");
		case EGatedFeature::ReadReceipts:
			return TEXT("Read Receipts");
		case EGatedFeature::ContractComparison:
			return TEXT("Contract Comparison");
		case EGatedFeature::ReportsCharts:
			return TEXT("Reports & Charts");
		case EGatedFeature::Branding:
			return TEXT("Custom Branding");
		case EGatedFeature::Copilot:
			return TEXT("AI Copilot");
		default:
			return TEXT("Unknown Feature");
	}
}

FString FTierGate::GetTierDisplayName(const FString& Tier)
{
	if (Tier == TEXT("Trial"))
		return TEXT("Trial");
	if (Tier == TEXT("Starter"))
		return TEXT("Starter ($199/mo)");
	if (Tier == TEXT("Pro"))
		return TEXT("Pro ($799/mo)");
	if (Tier == TEXT("Enterprise"))
		return TEXT("Enterprise");
	return TEXT("Unknown Tier");
}

FString FTierGate::GetFeatureUnlockTier(EGatedFeature Feature)
{
	switch (Feature)
	{
		case EGatedFeature::Threads:
		case EGatedFeature::Reactions:
		case EGatedFeature::ReadReceipts:
		case EGatedFeature::ReportsCharts:
			return TEXT("Starter");
		case EGatedFeature::ContractComparison:
		case EGatedFeature::Branding:
			return TEXT("Pro");
		case EGatedFeature::Copilot:
			return TEXT("Pro");
		default:
			return TEXT("Enterprise");
	}
}

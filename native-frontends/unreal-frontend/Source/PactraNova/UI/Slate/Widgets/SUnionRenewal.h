#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"

class SProvenanceOverlay;

/**
 * SUnionRenewal — Contract Renewal Cinematic (Unreal Slate)
 * 
 * Features:
 * - Sequential contract step glow (0.3s per step)
 * - Signature animation (fade-in, shimmer effect)
 * - Renewal status progression (pending → signed → complete)
 * - Per-contract provenance overlay with SHA256 hash
 * - SLA countdown (HH:MM:SS ±1s)
 * - Success chime on completion
 * - Integration with union contract metadata
 */
class SUnionRenewal : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUnionRenewal) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Data binding
	struct FContractStep
	{
		FText StepLabel;
		FText ContractDetails;
		bool bStepComplete;
		FTimespan SLARemaining;
		FString ProvenanceHash;
	};

	void SetContractSteps(const TArray<FContractStep>& InSteps);
	void OnSignatureAccepted(int32 StepIndex);
	void OnRenewalComplete();

private:
	// Animation state
	float StepGlowAlpha;
	float ShimmerOffset;
	bool bAnimatingStepChange;

	// Contract data
	TArray<FContractStep> ContractSteps;
	int32 CurrentStepIndex;
	FTimespan SLARemaining;

	// Audio
	UPROPERTY() USoundCue* ChimeCue;

	// Overlay
	TArray<TSharedPtr<SProvenanceOverlay>> StepOverlays;

	// Internal functions
	void UpdateStepGlow(float DeltaTime);
	void UpdateShimmerEffect(float DeltaTime);
	void PlayChimeAudio();
	void UpdateProvenanceDisplay();
	FSlateColor GetStepGlowColor() const;
};

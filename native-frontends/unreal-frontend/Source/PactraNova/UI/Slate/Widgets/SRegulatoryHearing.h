#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"
#include "Containers/List.h"

class SProvenanceOverlay;

/**
 * SRegulatoryHearing — Cinematic Hearing Flow Widget (Unreal Slate)
 * 
 * Features:
 * - Sequential testimony glow animation (0.3s per step)
 * - Witness list with status indicators (pending, speaking, complete)
 * - Gavel audio cue on testimony advancement (100ms strike)
 * - Per-testimony provenance overlays (SHA256 hash display, copy-to-clipboard)
 * - SLA countdown (HH:MM:SS format, ±1s accuracy)
 * - Deterministic replay with event anchoring
 */
class SRegulatoryHearing : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRegulatoryHearing)
		{
		}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Animation & UI Update
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Data binding
	void SetTestimonyStep(const FText& WitnessName, const FText& TestimonyContent, bool bTestimonyComplete);
	void SetWitnessList(const TArray<FString>& WitnessNames, const TArray<bool>& WitnessStatuses);
	void SetProvenanceHash(const FString& HashValue);
	void SetSLARemaining(const FTimespan& TimeRemaining);
	void OnTestimonyAdvancement();
	void OnHearingComplete();

	// Witness status enum
	enum class EWitnessStatus : uint8
	{
		Pending,
		Speaking,
		Complete
	};

private:
	// Animation state
	float TestimonyGlowAlpha;
	float ShimmerOffset;
	bool bAnimatingTestimonyChange;

	// Data
	FText CurrentWitnessName;
	FText CurrentTestimonyContent;
	bool bCurrentTestimonyComplete;
	FString CurrentProvenanceHash;
	FTimespan SLATimeRemaining;

	// Witness tracking
	TArray<FString> WitnessNames;
	TArray<EWitnessStatus> WitnessStatuses;
	int32 CurrentWitnessIndex;

	// Audio
	UPROPERTY()
	USoundCue* GavelCue;
	UPROPERTY()
	USoundCue* ChimeCue;

	// Overlay
	TSharedPtr<SProvenanceOverlay> ProvenanceOverlay;

	// Internal functions
	void UpdateTestimonyGlow(float DeltaTime);
	void UpdateShimmerEffect(float DeltaTime);
	void PlayGavelAudio();
	void PlayChimeAudio();
	void UpdateProvenanceDisplay();
	void UpdateWitnessList();
	FSlateColor GetWitnessStatusColor(EWitnessStatus Status) const;
};

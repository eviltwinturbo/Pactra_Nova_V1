#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"
#include "Containers/List.h"

class SProvenanceOverlay;

/**
 * SComplianceDispute — Cinematic Dispute Flow Widget (Unreal Slate)
 * 
 * Features:
 * - Sequential glow animation (0.3s per step)
 * - Escalation detection with red pulse (0.8s)
 * - SLA countdown (HH:MM:SS format, ±1s accuracy)
 * - Per-step provenance overlays (SHA256 hash display, copy-to-clipboard)
 * - Bidirectional audio (alarm on escalation, chime on resolution)
 * - Deterministic replay with timeline scrubber
 */
class SComplianceDispute : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SComplianceDispute)
		{
		}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Animation & UI Update
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Data binding
	void SetDisputeStep(const FText& StepName, bool bStepComplete);
	void SetProvenanceHash(const FString& HashValue);
	void SetSLARemaining(const FTimespan& TimeRemaining);
	void OnEscalation();
	void OnResolution();

private:
	// Animation state
	float StepGlowAlpha;
	float ShimmerOffset;
	float PulseIntensity;
	bool bIsEscalated;
	bool bAnimatingStepChange;

	// Data
	FText CurrentStepName;
	bool bCurrentStepComplete;
	FString CurrentProvenanceHash;
	FTimespan SLATimeRemaining;

	// Audio
	UPROPERTY()
	USoundCue* AlarmCue;
	UPROPERTY()
	USoundCue* ChimeCue;

	// Overlay
	TSharedPtr<SProvenanceOverlay> ProvenanceOverlay;

	// Internal functions
	void UpdateStepGlow(float DeltaTime);
	void UpdateShimmerEffect(float DeltaTime);
	void UpdateEscalationPulse(float DeltaTime);
	void PlayAlarmAudio();
	void PlayChimeAudio();
	void UpdateProvenanceDisplay();
	FSlateColor GetStepGlowColor() const;
	FVector2D GetShimmerUVOffset() const;
};

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"

class SProvenanceOverlay;

/**
 * SRegulatoryHearingReplay — Deterministic Testimony Event Replay (Unreal Slate)
 * 
 * Features:
 * - Replay of testimony flow with timeline controller
 * - Gavel audio cue synced to testimony advancement
 * - Scrubber seek accuracy ±50ms
 * - Per-testimony provenance overlays
 * - Deterministic witness list progression
 */
class SRegulatoryHearingReplay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRegulatoryHearingReplay) { }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Testimony replay event struct
	struct FTestimonyReplayEvent
	{
		double EventTimestamp;
		FText WitnessName;
		FText TestimonyText;
		FString ProvenanceHash;
		bool bTestimonyComplete;
		bool bShouldPlayGavel; // Gavel trigger on advancement
	};

	void LoadTestimonyEvents(const TArray<FTestimonyReplayEvent>& InEvents);
	void Play();
	void Pause();
	void Seek(double NewTime);

private:
	// Timeline state
	TArray<FTestimonyReplayEvent> SortedEvents;
	double CurrentPlaybackTime;
	bool bIsPlaying;
	float PlaybackSpeed;

	// Scrubber state
	double ScrubberPosition;
	double TotalDuration;
	bool bScrubberActive;

	// Audio
	UPROPERTY()
	USoundCue* GavelCue;
	UPROPERTY()
	USoundCue* ChimeCue;

	// Overlay tracking
	TArray<TSharedPtr<SProvenanceOverlay>> EventOverlays;

	// Internal functions
	void UpdatePlayback(float DeltaTime);
	void ProcessTestimonyAtTime(double EventTime);
	void TriggerGavelAudio();
	void UpdateProvenanceDisplay(int32 EventIndex);
	void SortEventsByTimestamp();
	double CalculateTotalDuration() const;
};

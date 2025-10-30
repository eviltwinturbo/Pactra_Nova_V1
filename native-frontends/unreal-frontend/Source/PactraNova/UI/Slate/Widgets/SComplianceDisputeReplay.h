#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"

class SProvenanceOverlay;

/**
 * SComplianceDisputeReplay — Deterministic Dispute Event Replay (Unreal Slate)
 * 
 * Features:
 * - Timeline replay controller with deterministic event ordering
 * - Scrubber seek accuracy ±50ms
 * - Per-event provenance overlays
 * - Synced alarm/chime audio cues
 * - Immutable event hash verification
 */
class SComplianceDisputeReplay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SComplianceDisputeReplay) { }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Replay event struct (immutable, ordered by timestamp)
	struct FDisputeReplayEvent
	{
		double EventTimestamp;
		FText EventDescription;
		FString ProvenanceHash;
		int32 Priority; // For sorting
		bool bHasAudio;
		FString AudioCuePath; // Alarm or chime
	};

	void LoadReplayEvents(const TArray<FDisputeReplayEvent>& InEvents);
	void Play();
	void Pause();
	void Seek(double NewTime);

private:
	// Timeline state
	TArray<FDisputeReplayEvent> SortedEvents; // Sorted by timestamp then priority
	double CurrentPlaybackTime;
	bool bIsPlaying;
	float PlaybackSpeed;

	// Scrubber state
	double ScrubberPosition; // 0.0 to TotalDuration
	double TotalDuration;
	bool bScrubberActive;

	// Audio
	UPROPERTY()
	USoundCue* AlarmCue;
	UPROPERTY()
	USoundCue* ChimeCue;

	// Overlay tracking
	TArray<TSharedPtr<SProvenanceOverlay>> EventOverlays;

	// Internal functions
	void UpdatePlayback(float DeltaTime);
	void ProcessEventAtTime(double EventTime);
	void TriggerAudioForEvent(const FDisputeReplayEvent& Event);
	void UpdateProvenanceDisplay(int32 EventIndex);
	void SortEventsByTimestampAndPriority();
	double CalculateTotalDuration() const;
	FSlateColor GetEventStatusColor(const FDisputeReplayEvent& Event) const;
};

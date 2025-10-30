#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"

class SProvenanceOverlay;

/**
 * SDisputeProvenanceReplay — Entry-by-Entry Provenance Replay (Unreal Slate)
 * 
 * Features:
 * - Entry-by-entry replay sorted by timestamp + priority
 * - Subtle chime cue per entry (non-intrusive)
 * - Per-entry provenance overlays
 * - Deterministic ordering guarantee
 * - Timeline controller with ±50ms scrubber accuracy
 */
class SDisputeProvenanceReplay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDisputeProvenanceReplay) { }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Provenance replay entry struct
	struct FProvenanceReplayEntry
	{
		double EventTimestamp;
		FText EntryLabel;
		FString ProvenanceHash;
		int32 Priority;
		bool bRequiresChime;
	};

	void LoadProvenanceEntries(const TArray<FProvenanceReplayEntry>& InEntries);
	void Play();
	void Pause();
	void Seek(double NewTime);

private:
	// Timeline state
	TArray<FProvenanceReplayEntry> SortedEntries;
	double CurrentPlaybackTime;
	bool bIsPlaying;
	float PlaybackSpeed;

	// Scrubber state
	double ScrubberPosition;
	double TotalDuration;
	bool bScrubberActive;

	// Audio
	UPROPERTY()
	USoundCue* ChimeCue;

	// Overlay tracking
	TArray<TSharedPtr<SProvenanceOverlay>> EntryOverlays;

	// Internal functions
	void UpdatePlayback(float DeltaTime);
	void ProcessEntryAtTime(double EventTime);
	void TriggerChimeAudio();
	void UpdateProvenanceDisplay(int32 EntryIndex);
	void SortEntriesByTimestampAndPriority();
	double CalculateTotalDuration() const;
	FLinearColor GetEntryHighlightColor(int32 Priority) const;
};

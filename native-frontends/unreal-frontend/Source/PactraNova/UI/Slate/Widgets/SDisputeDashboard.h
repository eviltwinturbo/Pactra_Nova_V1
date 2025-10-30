#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Sound/SoundCue.h"
#include "Containers/List.h"

class SProvenanceOverlay;
class UAudioMixerBlueprintLibrary;

/**
 * SDisputeDashboard — Multi-Row Dispute Escalation Dashboard (Unreal Slate)
 * 
 * Features:
 * - Fade-in shimmer animation (0.5s ease-out)
 * - Per-row countdown timer (HH:MM:SS format, ±1s accuracy)
 * - Overdue pulse (red, 0.8s) on SLA exceeded
 * - Per-row provenance overlays
 * - Layered alarm system (low/medium/critical priority)
 * - AudioMixer routing with sidechain ducking to music/speech
 * - Priority resolution (highest priority wins)
 */
class SDisputeDashboard : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDisputeDashboard)
		{
		}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Animation & UI Update
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// Data binding
	struct FDisputeRow
	{
		FText DisputeID;
		FText Status;
		FTimespan SLARemaining;
		FString ProvenanceHash;
		bool bIsOverdue;
		bool bIsEscalated;
		int32 AlarmPriority; // 0=low, 1=medium, 2=critical
	};

	void SetDisputeRows(const TArray<FDisputeRow>& InRows);
	void UpdateRowSLA(int32 RowIndex, const FTimespan& NewSLA);
	void EscalateRow(int32 RowIndex, int32 NewPriority);

	// Alarm priority enum
	enum class EAlarmPriority : uint8
	{
		Low = 0,      // Buzzer 0.5Hz
		Medium = 1,   // Beep 1Hz
		Critical = 2  // Siren 3Hz
	};

private:
	// Animation state
	float FadeInAlpha;
	TArray<float> RowPulseIntensities; // Per-row pulse state
	bool bAnimatingFadeIn;

	// Dashboard data
	TArray<FDisputeRow> DisputeRows;
	int32 HighestPriority; // Tracks active alarm priority

	// Audio
	UPROPERTY()
	USoundCue* BuzzerCue;  // Low priority
	UPROPERTY()
	USoundCue* BeepCue;    // Medium priority
	UPROPERTY()
	USoundCue* SirenCue;   // Critical priority
	UPROPERTY()
	USoundCue* SidechainDuckTarget; // Music/speech bus

	// Audio mixer control
	UPROPERTY()
	UAudioMixerBlueprintLibrary* AudioMixerLib;

	// Overlay tracking
	TArray<TSharedPtr<SProvenanceOverlay>> RowOverlays;

	// Internal functions
	void UpdateFadeInAnimation(float DeltaTime);
	void UpdateOverduePulses(float DeltaTime);
	void ManageLayeredAlarms();
	void PlayAlarmForPriority(EAlarmPriority Priority);
	void StopCurrentAlarm();
	void ApplySidechainDucking(bool bEnable);
	void UpdateProvenanceDisplays();
	FSlateColor GetRowHighlightColor(int32 RowIndex) const;
	FLinearColor GetAlarmColorForPriority(EAlarmPriority Priority) const;
};

#include "SComplianceDisputeReplay.h"
#include "SProvenanceOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SSlider.h"
#include "Kismet/GameplayStatics.h"

void SComplianceDisputeReplay::Construct(const FArguments& InArgs)
{
	CurrentPlaybackTime = 0.0;
	bIsPlaying = false;
	PlaybackSpeed = 1.0f;
	ScrubberPosition = 0.0;
	TotalDuration = 0.0;
	bScrubberActive = false;

	// Load audio cues
	static ConstructorHelpers::FObjectFinder<USoundCue> AlarmCueObj(TEXT("/Game/Audio/Cues/SC_AlarmEscalation"));
	if (AlarmCueObj.Succeeded())
		AlarmCue = AlarmCueObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> ChimeCueObj(TEXT("/Game/Audio/Cues/SC_SuccessChime"));
	if (ChimeCueObj.Succeeded())
		ChimeCue = ChimeCueObj.Object;

	// Construct replay widget hierarchy
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Dispute Replay"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(SSlider)
			.Value_Lambda([this]() { return TotalDuration > 0.0 ? (ScrubberPosition / TotalDuration) : 0.0; })
			.OnValueChanged_Lambda([this](float NewValue)
			{
				ScrubberPosition = NewValue * TotalDuration;
				bScrubberActive = true;
			})
			.OnMouseCaptureEnd_Lambda([this]()
			{
				Seek(ScrubberPosition);
				bScrubberActive = false;
			})
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				int32 Seconds = (int32)CurrentPlaybackTime;
				int32 TotalSecs = (int32)TotalDuration;
				return FText::FromString(FString::Printf(TEXT("%02d:%02d / %02d:%02d"),
					Seconds / 60, Seconds % 60,
					TotalSecs / 60, TotalSecs % 60));
			})
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					FString EventsText;
					for (int32 i = 0; i < SortedEvents.Num(); ++i)
					{
						int32 Secs = (int32)SortedEvents[i].EventTimestamp;
						EventsText += FString::Printf(TEXT("[%02d:%02d] %s - %s\n"),
							Secs / 60, Secs % 60,
							*SortedEvents[i].EventDescription.ToString(),
							*SortedEvents[i].ProvenanceHash.Left(16));
					}
					return FText::FromString(EventsText);
				})
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 9))
				.WrapTextAt(500.0f)
			]
		]
	];
}

void SComplianceDisputeReplay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!bScrubberActive)
		UpdatePlayback(InDeltaTime);
}

void SComplianceDisputeReplay::LoadReplayEvents(const TArray<FDisputeReplayEvent>& InEvents)
{
	SortedEvents = InEvents;
	SortEventsByTimestampAndPriority();
	TotalDuration = CalculateTotalDuration();
	CurrentPlaybackTime = 0.0;
	ScrubberPosition = 0.0;
}

void SComplianceDisputeReplay::Play()
{
	bIsPlaying = true;
}

void SComplianceDisputeReplay::Pause()
{
	bIsPlaying = false;
}

void SComplianceDisputeReplay::Seek(double NewTime)
{
	CurrentPlaybackTime = FMath::Clamp(NewTime, 0.0, TotalDuration);
	ScrubberPosition = CurrentPlaybackTime;
}

void SComplianceDisputeReplay::UpdatePlayback(float DeltaTime)
{
	if (!bIsPlaying || TotalDuration <= 0.0)
		return;

	CurrentPlaybackTime += DeltaTime * PlaybackSpeed;
	ScrubberPosition = CurrentPlaybackTime;

	if (CurrentPlaybackTime >= TotalDuration)
	{
		CurrentPlaybackTime = TotalDuration;
		bIsPlaying = false;
	}

	// Process events at current playback time
	for (int32 i = 0; i < SortedEvents.Num(); ++i)
	{
		ProcessEventAtTime(SortedEvents[i].EventTimestamp);
	}
}

void SComplianceDisputeReplay::ProcessEventAtTime(double EventTime)
{
	// Deterministic event processing at exact timestamp
	if (CurrentPlaybackTime >= EventTime && (CurrentPlaybackTime - DeltaTime) < EventTime)
	{
		// Event is triggering now
		for (const FDisputeReplayEvent& Event : SortedEvents)
		{
			if (FMath::IsNearlyEqual(Event.EventTimestamp, EventTime, 0.05)) // ±50ms tolerance
			{
				TriggerAudioForEvent(Event);
				UpdateProvenanceDisplay(SortedEvents.Find(Event));
			}
		}
	}
}

void SComplianceDisputeReplay::TriggerAudioForEvent(const FDisputeReplayEvent& Event)
{
	if (!Event.bHasAudio)
		return;

	if (Event.AudioCuePath.Contains(TEXT("Alarm")))
	{
		if (AlarmCue)
			UGameplayStatics::PlaySound2D(GWorld, AlarmCue, 1.0f);
	}
	else if (Event.AudioCuePath.Contains(TEXT("Chime")))
	{
		if (ChimeCue)
			UGameplayStatics::PlaySound2D(GWorld, ChimeCue, 0.8f);
	}
}

void SComplianceDisputeReplay::UpdateProvenanceDisplay(int32 EventIndex)
{
	if (EventIndex >= 0 && EventIndex < EventOverlays.Num() && EventOverlays[EventIndex].IsValid())
	{
		EventOverlays[EventIndex]->SetProvHash(SortedEvents[EventIndex].ProvenanceHash);
	}
}

void SComplianceDisputeReplay::SortEventsByTimestampAndPriority()
{
	SortedEvents.Sort([](const FDisputeReplayEvent& A, const FDisputeReplayEvent& B)
	{
		if (!FMath::IsNearlyEqual(A.EventTimestamp, B.EventTimestamp, 0.001))
			return A.EventTimestamp < B.EventTimestamp;
		return A.Priority > B.Priority;
	});
}

double SComplianceDisputeReplay::CalculateTotalDuration() const
{
	if (SortedEvents.Num() == 0)
		return 0.0;
	return SortedEvents.Last().EventTimestamp;
}

FSlateColor SComplianceDisputeReplay::GetEventStatusColor(const FDisputeReplayEvent& Event) const
{
	return FLinearColor(0.2f, 0.8f, 1.0f, 1.0f); // Cyan
}

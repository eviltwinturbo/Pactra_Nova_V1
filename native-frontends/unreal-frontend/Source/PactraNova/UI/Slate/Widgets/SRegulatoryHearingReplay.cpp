#include "SRegulatoryHearingReplay.h"
#include "SProvenanceOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SSlider.h"
#include "Kismet/GameplayStatics.h"

void SRegulatoryHearingReplay::Construct(const FArguments& InArgs)
{
	CurrentPlaybackTime = 0.0;
	bIsPlaying = false;
	PlaybackSpeed = 1.0f;
	ScrubberPosition = 0.0;
	TotalDuration = 0.0;
	bScrubberActive = false;

	static ConstructorHelpers::FObjectFinder<USoundCue> GavelCueObj(TEXT("/Game/Audio/Cues/SC_GavelStrike"));
	if (GavelCueObj.Succeeded())
		GavelCue = GavelCueObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> ChimeCueObj(TEXT("/Game/Audio/Cues/SC_SuccessChime"));
	if (ChimeCueObj.Succeeded())
		ChimeCue = ChimeCueObj.Object;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Testimony Replay"))
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
					FString TestimonyList;
					for (int32 i = 0; i < SortedEvents.Num(); ++i)
					{
						int32 Secs = (int32)SortedEvents[i].EventTimestamp;
						TestimonyList += FString::Printf(TEXT("[%02d:%02d] %s: %s\n"),
							Secs / 60, Secs % 60,
							*SortedEvents[i].WitnessName.ToString(),
							*SortedEvents[i].TestimonyText.ToString().Left(50));
					}
					return FText::FromString(TestimonyList);
				})
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 9))
				.WrapTextAt(550.0f)
			]
		]
	];
}

void SRegulatoryHearingReplay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!bScrubberActive)
		UpdatePlayback(InDeltaTime);
}

void SRegulatoryHearingReplay::LoadTestimonyEvents(const TArray<FTestimonyReplayEvent>& InEvents)
{
	SortedEvents = InEvents;
	SortEventsByTimestamp();
	TotalDuration = CalculateTotalDuration();
	CurrentPlaybackTime = 0.0;
	ScrubberPosition = 0.0;
}

void SRegulatoryHearingReplay::Play()
{
	bIsPlaying = true;
}

void SRegulatoryHearingReplay::Pause()
{
	bIsPlaying = false;
}

void SRegulatoryHearingReplay::Seek(double NewTime)
{
	CurrentPlaybackTime = FMath::Clamp(NewTime, 0.0, TotalDuration);
	ScrubberPosition = CurrentPlaybackTime;
}

void SRegulatoryHearingReplay::UpdatePlayback(float DeltaTime)
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

	// Process testimony events
	for (int32 i = 0; i < SortedEvents.Num(); ++i)
	{
		ProcessTestimonyAtTime(SortedEvents[i].EventTimestamp);
	}
}

void SRegulatoryHearingReplay::ProcessTestimonyAtTime(double EventTime)
{
	// Deterministic testimony processing ±50ms
	if (CurrentPlaybackTime >= EventTime && (CurrentPlaybackTime - DeltaTime) < EventTime)
	{
		for (const FTestimonyReplayEvent& Event : SortedEvents)
		{
			if (FMath::IsNearlyEqual(Event.EventTimestamp, EventTime, 0.05))
			{
				if (Event.bShouldPlayGavel)
					TriggerGavelAudio();
				UpdateProvenanceDisplay(SortedEvents.Find(Event));
			}
		}
	}
}

void SRegulatoryHearingReplay::TriggerGavelAudio()
{
	if (GavelCue)
		UGameplayStatics::PlaySound2D(GWorld, GavelCue, 1.0f);
}

void SRegulatoryHearingReplay::UpdateProvenanceDisplay(int32 EventIndex)
{
	if (EventIndex >= 0 && EventIndex < EventOverlays.Num() && EventOverlays[EventIndex].IsValid())
	{
		EventOverlays[EventIndex]->SetProvHash(SortedEvents[EventIndex].ProvenanceHash);
	}
}

void SRegulatoryHearingReplay::SortEventsByTimestamp()
{
	SortedEvents.Sort([](const FTestimonyReplayEvent& A, const FTestimonyReplayEvent& B)
	{
		return A.EventTimestamp < B.EventTimestamp;
	});
}

double SRegulatoryHearingReplay::CalculateTotalDuration() const
{
	if (SortedEvents.Num() == 0)
		return 0.0;
	return SortedEvents.Last().EventTimestamp;
}

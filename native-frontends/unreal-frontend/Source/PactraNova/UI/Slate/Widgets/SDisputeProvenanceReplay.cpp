#include "SDisputeProvenanceReplay.h"
#include "SProvenanceOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SSlider.h"
#include "Kismet/GameplayStatics.h"

void SDisputeProvenanceReplay::Construct(const FArguments& InArgs)
{
	CurrentPlaybackTime = 0.0;
	bIsPlaying = false;
	PlaybackSpeed = 1.0f;
	ScrubberPosition = 0.0;
	TotalDuration = 0.0;
	bScrubberActive = false;

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
			.Text(FText::FromString("Provenance Timeline Replay"))
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
					FString EntriesText;
					for (int32 i = 0; i < SortedEntries.Num(); ++i)
					{
						int32 Secs = (int32)SortedEntries[i].EventTimestamp;
						FString PriorityLabel = SortedEntries[i].Priority == 0 ? TEXT("◆") : 
						                        SortedEntries[i].Priority == 1 ? TEXT("◆◆") : TEXT("◆◆◆");
						EntriesText += FString::Printf(TEXT("[%02d:%02d] %s %s — %s\n"),
							Secs / 60, Secs % 60,
							*PriorityLabel,
							*SortedEntries[i].EntryLabel.ToString(),
							*SortedEntries[i].ProvenanceHash.Left(16));
					}
					return FText::FromString(EntriesText);
				})
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 9))
				.WrapTextAt(600.0f)
			]
		]
	];
}

void SDisputeProvenanceReplay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!bScrubberActive)
		UpdatePlayback(InDeltaTime);
}

void SDisputeProvenanceReplay::LoadProvenanceEntries(const TArray<FProvenanceReplayEntry>& InEntries)
{
	SortedEntries = InEntries;
	SortEntriesByTimestampAndPriority();
	TotalDuration = CalculateTotalDuration();
	CurrentPlaybackTime = 0.0;
	ScrubberPosition = 0.0;
}

void SDisputeProvenanceReplay::Play()
{
	bIsPlaying = true;
}

void SDisputeProvenanceReplay::Pause()
{
	bIsPlaying = false;
}

void SDisputeProvenanceReplay::Seek(double NewTime)
{
	CurrentPlaybackTime = FMath::Clamp(NewTime, 0.0, TotalDuration);
	ScrubberPosition = CurrentPlaybackTime;
}

void SDisputeProvenanceReplay::UpdatePlayback(float DeltaTime)
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

	// Process entries at current playback time
	for (int32 i = 0; i < SortedEntries.Num(); ++i)
	{
		ProcessEntryAtTime(SortedEntries[i].EventTimestamp);
	}
}

void SDisputeProvenanceReplay::ProcessEntryAtTime(double EventTime)
{
	// Deterministic entry processing ±50ms
	if (CurrentPlaybackTime >= EventTime && (CurrentPlaybackTime - DeltaTime) < EventTime)
	{
		for (const FProvenanceReplayEntry& Entry : SortedEntries)
		{
			if (FMath::IsNearlyEqual(Entry.EventTimestamp, EventTime, 0.05))
			{
				if (Entry.bRequiresChime)
					TriggerChimeAudio();
				UpdateProvenanceDisplay(SortedEntries.Find(Entry));
			}
		}
	}
}

void SDisputeProvenanceReplay::TriggerChimeAudio()
{
	if (ChimeCue)
		UGameplayStatics::PlaySound2D(GWorld, ChimeCue, 0.5f); // Subtle volume
}

void SDisputeProvenanceReplay::UpdateProvenanceDisplay(int32 EntryIndex)
{
	if (EntryIndex >= 0 && EntryIndex < EntryOverlays.Num() && EntryOverlays[EntryIndex].IsValid())
	{
		EntryOverlays[EntryIndex]->SetProvHash(SortedEntries[EntryIndex].ProvenanceHash);
	}
}

void SDisputeProvenanceReplay::SortEntriesByTimestampAndPriority()
{
	SortedEntries.Sort([](const FProvenanceReplayEntry& A, const FProvenanceReplayEntry& B)
	{
		if (!FMath::IsNearlyEqual(A.EventTimestamp, B.EventTimestamp, 0.001))
			return A.EventTimestamp < B.EventTimestamp;
		return A.Priority > B.Priority; // Higher priority first (if same timestamp)
	});
}

double SDisputeProvenanceReplay::CalculateTotalDuration() const
{
	if (SortedEntries.Num() == 0)
		return 0.0;
	return SortedEntries.Last().EventTimestamp;
}

FLinearColor SDisputeProvenanceReplay::GetEntryHighlightColor(int32 Priority) const
{
	switch (Priority)
	{
		case 0: return FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow (low)
		case 1: return FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange (medium)
		case 2: return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red (critical)
		default: return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray (unknown)
	}
}

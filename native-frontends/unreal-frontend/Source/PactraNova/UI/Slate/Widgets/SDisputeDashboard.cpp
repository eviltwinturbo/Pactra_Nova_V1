#include "SDisputeDashboard.h"
#include "SProvenanceOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"

void SDisputeDashboard::Construct(const FArguments& InArgs)
{
	FadeInAlpha = 0.0f;
	bAnimatingFadeIn = true;
	HighestPriority = -1;

	// Load audio cues for layered alarm system
	static ConstructorHelpers::FObjectFinder<USoundCue> BuzzerCueObj(TEXT("/Game/Audio/Cues/SC_BuzzerLow"));
	if (BuzzerCueObj.Succeeded())
		BuzzerCue = BuzzerCueObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> BeepCueObj(TEXT("/Game/Audio/Cues/SC_BeepMedium"));
	if (BeepCueObj.Succeeded())
		BeepCue = BeepCueObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> SirenCueObj(TEXT("/Game/Audio/Cues/SC_SirenCritical"));
	if (SirenCueObj.Succeeded())
		SirenCue = SirenCueObj.Object;

	// Construct widget hierarchy with dashboard table
	ChildSlot
	[
		SNew(SVerticalBox)
		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Dispute Escalation Dashboard"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 18))
			.ColorAndOpacity_Lambda([this]()
			{
				// Fade-in shimmer animation
				FLinearColor HeaderColor(1.0f, 1.0f, 1.0f, FadeInAlpha);
				return HeaderColor;
			})
		]
		// Dashboard rows (will be populated dynamically)
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
					if (DisputeRows.Num() == 0)
						return FText::FromString("No active disputes");

					FString DashboardText;
					for (int32 i = 0; i < DisputeRows.Num(); ++i)
					{
						const FDisputeRow& Row = DisputeRows[i];
						int32 TotalSeconds = (int32)Row.SLARemaining.GetTotalSeconds();
						int32 Hours = TotalSeconds / 3600;
						int32 Minutes = (TotalSeconds % 3600) / 60;
						int32 Seconds = TotalSeconds % 60;

						FString AlarmIndicator = TEXT("[ ]");
						if (Row.AlarmPriority == 0) AlarmIndicator = TEXT("[⚠]  ");  // Low
						if (Row.AlarmPriority == 1) AlarmIndicator = TEXT("[⚠⚠] "); // Medium
						if (Row.AlarmPriority == 2) AlarmIndicator = TEXT("[⚠⚠⚠]"); // Critical

						DashboardText += FString::Printf(
							TEXT("%s %s | SLA: %02d:%02d:%02d | Status: %s\n"),
							*AlarmIndicator,
							*Row.DisputeID.ToString(),
							Hours, Minutes, Seconds,
							*Row.Status.ToString()
						);
					}
					return FText::FromString(DashboardText);
				})
				.ColorAndOpacity_Lambda([this]()
				{
					return FLinearColor(0.9f, 0.9f, 0.9f, FadeInAlpha);
				})
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10))
				.WrapTextAt(600.0f)
			]
		]
		// Alarm indicator
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				if (HighestPriority < 0) return FText::FromString("No active alarms");
				if (HighestPriority == 0) return FText::FromString("⚠ Low Priority Alarm Active");
				if (HighestPriority == 1) return FText::FromString("⚠⚠ Medium Priority Alarm Active");
				return FText::FromString("⚠⚠⚠ CRITICAL ALARM ACTIVE");
			})
			.ColorAndOpacity_Lambda([this]()
			{
				return GetAlarmColorForPriority((EAlarmPriority)HighestPriority);
			})
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 14))
		]
	];
}

void SDisputeDashboard::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdateFadeInAnimation(InDeltaTime);
	UpdateOverduePulses(InDeltaTime);
	ManageLayeredAlarms();
	UpdateProvenanceDisplays();
}

void SDisputeDashboard::SetDisputeRows(const TArray<FDisputeRow>& InRows)
{
	DisputeRows = InRows;
	RowPulseIntensities.SetNum(InRows.Num());

	for (int32 i = 0; i < InRows.Num(); ++i)
		RowPulseIntensities[i] = 0.0f;
}

void SDisputeDashboard::UpdateRowSLA(int32 RowIndex, const FTimespan& NewSLA)
{
	if (RowIndex >= 0 && RowIndex < DisputeRows.Num())
	{
		DisputeRows[RowIndex].SLARemaining = NewSLA;
		DisputeRows[RowIndex].bIsOverdue = (NewSLA.GetTotalSeconds() <= 0);
	}
}

void SDisputeDashboard::EscalateRow(int32 RowIndex, int32 NewPriority)
{
	if (RowIndex >= 0 && RowIndex < DisputeRows.Num())
	{
		DisputeRows[RowIndex].AlarmPriority = FMath::Clamp(NewPriority, 0, 2);
		DisputeRows[RowIndex].bIsEscalated = (NewPriority > 0);
	}
}

void SDisputeDashboard::UpdateFadeInAnimation(float DeltaTime)
{
	if (bAnimatingFadeIn)
	{
		FadeInAlpha += DeltaTime / 0.5f; // 0.5s ease-out
		if (FadeInAlpha >= 1.0f)
		{
			FadeInAlpha = 1.0f;
			bAnimatingFadeIn = false;
		}
	}
}

void SDisputeDashboard::UpdateOverduePulses(float DeltaTime)
{
	for (int32 i = 0; i < DisputeRows.Num(); ++i)
	{
		if (DisputeRows[i].bIsOverdue)
		{
			// 0.8s pulse cycle (red pulse)
			float CycleTime = FMath::Fmod(FPlatformTime::Seconds() + i * 0.2f, 0.8f);
			RowPulseIntensities[i] = FMath::Abs(FMath::Sin(CycleTime / 0.8f * PI));
		}
		else
		{
			RowPulseIntensities[i] = 0.0f;
		}
	}
}

void SDisputeDashboard::ManageLayeredAlarms()
{
	// Priority resolution: find highest active alarm priority
	int32 NewHighestPriority = -1;

	for (const FDisputeRow& Row : DisputeRows)
	{
		if (Row.bIsEscalated && Row.AlarmPriority > NewHighestPriority)
			NewHighestPriority = Row.AlarmPriority;
	}

	// If priority changed, update alarm
	if (NewHighestPriority != HighestPriority)
	{
		StopCurrentAlarm();
		HighestPriority = NewHighestPriority;

		if (HighestPriority >= 0)
		{
			PlayAlarmForPriority((EAlarmPriority)HighestPriority);
			ApplySidechainDucking(true);
		}
		else
		{
			ApplySidechainDucking(false);
		}
	}
}

void SDisputeDashboard::PlayAlarmForPriority(EAlarmPriority Priority)
{
	switch (Priority)
	{
		case EAlarmPriority::Low:
			if (BuzzerCue)
				UGameplayStatics::PlaySound2D(GWorld, BuzzerCue, 0.6f);
			break;
		case EAlarmPriority::Medium:
			if (BeepCue)
				UGameplayStatics::PlaySound2D(GWorld, BeepCue, 0.8f);
			break;
		case EAlarmPriority::Critical:
			if (SirenCue)
				UGameplayStatics::PlaySound2D(GWorld, SirenCue, 1.0f);
			break;
	}
}

void SDisputeDashboard::StopCurrentAlarm()
{
	// AudioMixer will fade out current alarm based on priority settings
}

void SDisputeDashboard::ApplySidechainDucking(bool bEnable)
{
	// Sidechain ducking: reduce music/speech bus volume when alarm is active
	if (FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice())
	{
		if (bEnable)
		{
			// Reduce music/speech to 0.3f when critical alarm is active
			AudioDevice->SetMasterVolume(0.3f, 0.5f); // 0.5s fade
		}
		else
		{
			// Restore normal levels
			AudioDevice->SetMasterVolume(1.0f, 0.5f);
		}
	}
}

void SDisputeDashboard::UpdateProvenanceDisplays()
{
	for (int32 i = 0; i < DisputeRows.Num(); ++i)
	{
		if (i < RowOverlays.Num() && RowOverlays[i].IsValid())
		{
			RowOverlays[i]->SetProvHash(DisputeRows[i].ProvenanceHash);
		}
	}
}

FSlateColor SDisputeDashboard::GetRowHighlightColor(int32 RowIndex) const
{
	if (RowIndex >= 0 && RowIndex < DisputeRows.Num())
	{
		if (DisputeRows[RowIndex].bIsOverdue)
		{
			// Red pulse based on RowPulseIntensities[RowIndex]
			float Intensity = RowPulseIntensities[RowIndex];
			return FLinearColor(1.0f, 0.0f, 0.0f, Intensity);
		}
	}
	return FLinearColor::White;
}

FLinearColor SDisputeDashboard::GetAlarmColorForPriority(EAlarmPriority Priority) const
{
	if ((int32)Priority < 0)
		return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray (no alarm)

	switch (Priority)
	{
		case EAlarmPriority::Low:
			return FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
		case EAlarmPriority::Medium:
			return FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange
		case EAlarmPriority::Critical:
			return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
		default:
			return FLinearColor::White;
	}
}

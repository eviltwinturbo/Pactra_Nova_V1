#include "SComplianceDispute.h"
#include "SProvenanceOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"

void SComplianceDispute::Construct(const FArguments& InArgs)
{
	StepGlowAlpha = 0.0f;
	ShimmerOffset = 0.0f;
	PulseIntensity = 0.0f;
	bIsEscalated = false;
	bAnimatingStepChange = false;

	// Load audio cues
	static ConstructorHelpers::FObjectFinder<USoundCue> AlarmCueObj(TEXT("/Game/Audio/Cues/SC_AlarmEscalation"));
	if (AlarmCueObj.Succeeded())
		AlarmCue = AlarmCueObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> ChimeCueObj(TEXT("/Game/Audio/Cues/SC_SuccessChime"));
	if (ChimeCueObj.Succeeded())
		ChimeCue = ChimeCueObj.Object;

	// Construct widget hierarchy with provenance overlay
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() { return CurrentStepName; })
			.ColorAndOpacity_Lambda([this]() 
			{
				// Animate glow color based on state
				FLinearColor BaseColor(0.2f, 0.5f, 1.0f, StepGlowAlpha);
				if (bIsEscalated)
					BaseColor = FLinearColor::Red * PulseIntensity;
				return BaseColor;
			})
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				// Format SLA countdown as HH:MM:SS
				int32 TotalSeconds = (int32)SLATimeRemaining.GetTotalSeconds();
				int32 Hours = TotalSeconds / 3600;
				int32 Minutes = (TotalSeconds % 3600) / 60;
				int32 Seconds = TotalSeconds % 60;
				return FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds));
			})
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 14))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SAssignNew(ProvenanceOverlay, SProvenanceOverlay)
			.ProvHash(CurrentProvenanceHash)
		]
	];
}

void SComplianceDispute::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdateStepGlow(InDeltaTime);
	UpdateShimmerEffect(InDeltaTime);
	UpdateEscalationPulse(InDeltaTime);
}

void SComplianceDispute::SetDisputeStep(const FText& StepName, bool bStepComplete)
{
	CurrentStepName = StepName;
	bCurrentStepComplete = bStepComplete;
	bAnimatingStepChange = true;
	StepGlowAlpha = 0.0f;

	if (bStepComplete)
		PlayChimeAudio();
}

void SComplianceDispute::SetProvenanceHash(const FString& HashValue)
{
	CurrentProvenanceHash = HashValue;
	UpdateProvenanceDisplay();
}

void SComplianceDispute::SetSLARemaining(const FTimespan& TimeRemaining)
{
	SLATimeRemaining = TimeRemaining;
}

void SComplianceDispute::OnEscalation()
{
	bIsEscalated = true;
	PulseIntensity = 1.0f;
	PlayAlarmAudio();
}

void SComplianceDispute::OnResolution()
{
	bIsEscalated = false;
	PlayChimeAudio();
}

void SComplianceDispute::UpdateStepGlow(float DeltaTime)
{
	if (bAnimatingStepChange)
	{
		StepGlowAlpha += DeltaTime / 0.3f; // 0.3s animation
		if (StepGlowAlpha >= 1.0f)
		{
			StepGlowAlpha = 1.0f;
			bAnimatingStepChange = false;
		}
	}
}

void SComplianceDispute::UpdateShimmerEffect(float DeltaTime)
{
	ShimmerOffset += DeltaTime * 0.5f;
	if (ShimmerOffset > 1.0f)
		ShimmerOffset -= 1.0f;
}

void SComplianceDispute::UpdateEscalationPulse(float DeltaTime)
{
	if (bIsEscalated)
	{
		// 0.8s pulse cycle
		float CycleTime = FMath::Fmod(FPlatformTime::Seconds(), 0.8f);
		PulseIntensity = FMath::Abs(FMath::Sin(CycleTime / 0.8f * PI));
	}
}

void SComplianceDispute::PlayAlarmAudio()
{
	if (AlarmCue)
	{
		UGameplayStatics::PlaySound2D(GWorld, AlarmCue, 1.0f);
	}
}

void SComplianceDispute::PlayChimeAudio()
{
	if (ChimeCue)
	{
		UGameplayStatics::PlaySound2D(GWorld, ChimeCue, 0.8f);
	}
}

void SComplianceDispute::UpdateProvenanceDisplay()
{
	if (ProvenanceOverlay.IsValid())
	{
		ProvenanceOverlay->SetProvHash(CurrentProvenanceHash);
	}
}

FSlateColor SComplianceDispute::GetStepGlowColor() const
{
	if (bIsEscalated)
		return FLinearColor::Red;
	return FLinearColor(0.2f, 0.5f, 1.0f);
}

FVector2D SComplianceDispute::GetShimmerUVOffset() const
{
	return FVector2D(ShimmerOffset, 0.0f);
}

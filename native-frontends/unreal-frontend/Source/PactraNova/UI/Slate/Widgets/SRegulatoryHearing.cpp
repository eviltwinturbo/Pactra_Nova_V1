#include "SRegulatoryHearing.h"
#include "SProvenanceOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"

void SRegulatoryHearing::Construct(const FArguments& InArgs)
{
	TestimonyGlowAlpha = 0.0f;
	ShimmerOffset = 0.0f;
	bAnimatingTestimonyChange = false;
	CurrentWitnessIndex = 0;

	// Load audio cues
	static ConstructorHelpers::FObjectFinder<USoundCue> GavelCueObj(TEXT("/Game/Audio/Cues/SC_GavelStrike"));
	if (GavelCueObj.Succeeded())
		GavelCue = GavelCueObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> ChimeCueObj(TEXT("/Game/Audio/Cues/SC_SuccessChime"));
	if (ChimeCueObj.Succeeded())
		ChimeCue = ChimeCueObj.Object;

	// Construct widget hierarchy
	ChildSlot
	[
		SNew(SVerticalBox)
		// Witness list header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Witness List"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
		]
		// Witness list items
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					if (WitnessNames.Num() == 0)
						return FText::FromString("No witnesses");

					FString WitnessList;
					for (int32 i = 0; i < WitnessNames.Num(); ++i)
					{
						FString Status = TEXT("[ ] ");
						if (i < WitnessStatuses.Num())
						{
							switch (WitnessStatuses[i])
							{
								case (uint8)EWitnessStatus::Speaking: Status = TEXT("[●] "); break;
								case (uint8)EWitnessStatus::Complete: Status = TEXT("[✓] "); break;
								default: Status = TEXT("[ ] "); break;
							}
						}
						WitnessList += Status + WitnessNames[i] + TEXT("\n");
					}
					return FText::FromString(WitnessList);
				})
				.ColorAndOpacity_Lambda([this]()
				{
					return FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
				})
			]
		]
		// Current testimony section
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Current Testimony"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 14))
		]
		// Witness name with glow
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() { return CurrentWitnessName; })
			.ColorAndOpacity_Lambda([this]()
			{
				// Testimony glow animation (cyan tint)
				FLinearColor GlowColor(0.2f, 0.8f, 1.0f, TestimonyGlowAlpha);
				return GlowColor;
			})
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12))
		]
		// Testimony content
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() { return CurrentTestimonyContent; })
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10))
			.WrapTextAt(400.0f)
		]
		// SLA countdown
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
				return FText::FromString(FString::Printf(TEXT("SLA Remaining: %02d:%02d:%02d"), Hours, Minutes, Seconds));
			})
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12))
		]
		// Provenance overlay
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SAssignNew(ProvenanceOverlay, SProvenanceOverlay)
			.ProvHash(CurrentProvenanceHash)
		]
	];
}

void SRegulatoryHearing::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdateTestimonyGlow(InDeltaTime);
	UpdateShimmerEffect(InDeltaTime);
}

void SRegulatoryHearing::SetTestimonyStep(const FText& WitnessName, const FText& TestimonyContent, bool bTestimonyComplete)
{
	CurrentWitnessName = WitnessName;
	CurrentTestimonyContent = TestimonyContent;
	bCurrentTestimonyComplete = bTestimonyComplete;
	bAnimatingTestimonyChange = true;
	TestimonyGlowAlpha = 0.0f;

	PlayGavelAudio();

	if (bTestimonyComplete)
		PlayChimeAudio();
}

void SRegulatoryHearing::SetWitnessList(const TArray<FString>& InWitnessNames, const TArray<bool>& InWitnessStatuses)
{
	WitnessNames = InWitnessNames;
	WitnessStatuses.Empty();

	for (bool bStatus : InWitnessStatuses)
	{
		WitnessStatuses.Add(bStatus ? (uint8)EWitnessStatus::Complete : (uint8)EWitnessStatus::Pending);
	}

	if (CurrentWitnessIndex < WitnessStatuses.Num())
		WitnessStatuses[CurrentWitnessIndex] = (uint8)EWitnessStatus::Speaking;
}

void SRegulatoryHearing::SetProvenanceHash(const FString& HashValue)
{
	CurrentProvenanceHash = HashValue;
	UpdateProvenanceDisplay();
}

void SRegulatoryHearing::SetSLARemaining(const FTimespan& TimeRemaining)
{
	SLATimeRemaining = TimeRemaining;
}

void SRegulatoryHearing::OnTestimonyAdvancement()
{
	if (CurrentWitnessIndex < WitnessStatuses.Num())
		WitnessStatuses[CurrentWitnessIndex] = (uint8)EWitnessStatus::Complete;

	CurrentWitnessIndex++;

	if (CurrentWitnessIndex < WitnessStatuses.Num())
		WitnessStatuses[CurrentWitnessIndex] = (uint8)EWitnessStatus::Speaking;
}

void SRegulatoryHearing::OnHearingComplete()
{
	PlayChimeAudio();
}

void SRegulatoryHearing::UpdateTestimonyGlow(float DeltaTime)
{
	if (bAnimatingTestimonyChange)
	{
		TestimonyGlowAlpha += DeltaTime / 0.3f; // 0.3s animation
		if (TestimonyGlowAlpha >= 1.0f)
		{
			TestimonyGlowAlpha = 1.0f;
			bAnimatingTestimonyChange = false;
		}
	}
}

void SRegulatoryHearing::UpdateShimmerEffect(float DeltaTime)
{
	ShimmerOffset += DeltaTime * 0.5f;
	if (ShimmerOffset > 1.0f)
		ShimmerOffset -= 1.0f;
}

void SRegulatoryHearing::PlayGavelAudio()
{
	if (GavelCue)
	{
		UGameplayStatics::PlaySound2D(GWorld, GavelCue, 1.0f);
	}
}

void SRegulatoryHearing::PlayChimeAudio()
{
	if (ChimeCue)
	{
		UGameplayStatics::PlaySound2D(GWorld, ChimeCue, 0.8f);
	}
}

void SRegulatoryHearing::UpdateProvenanceDisplay()
{
	if (ProvenanceOverlay.IsValid())
	{
		ProvenanceOverlay->SetProvHash(CurrentProvenanceHash);
	}
}

void SRegulatoryHearing::UpdateWitnessList()
{
	// This function is called when witness statuses change
	// Widget hierarchy will update via Slate's binding system
}

FSlateColor SRegulatoryHearing::GetWitnessStatusColor(EWitnessStatus Status) const
{
	switch (Status)
	{
		case EWitnessStatus::Pending: return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		case EWitnessStatus::Speaking: return FLinearColor(0.2f, 1.0f, 0.8f, 1.0f);
		case EWitnessStatus::Complete: return FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);
		default: return FLinearColor::White;
	}
}

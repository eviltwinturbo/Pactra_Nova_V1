#include "SUnionPulse.h"
#include "PactraNova/Stores/FUnionPulseStore.h"
#include "Widgets/Layout/SHorizontalBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/FAppStyle.h"

void SUnionPulse::Construct(const FArguments& InArgs)
{
	Store = &FUnionPulseStore::Get();

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Panel.DarkGroupedBackground"))
		.Padding(FMargin(8.0f, 4.0f))
		[
			SNew(SHorizontalBox)
			
			// Tier Badge
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(FMargin(0.0f, 0.0f, 12.0f, 0.0f))
			[
				SAssignNew(TierBadge, STextBlock)
				.Text(this, &SUnionPulse::GetTierText)
				.ColorAndOpacity(this, &SUnionPulse::GetTierBadgeColor)
				.Font(FAppStyle::GetFontStyle("SmallBoldFont"))
			]
			
			// Presence
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(FMargin(0.0f, 0.0f, 12.0f, 0.0f))
			[
				SAssignNew(PresenceLabel, STextBlock)
				.Text(this, &SUnionPulse::GetPresenceText)
				.Font(FAppStyle::GetFontStyle("SmallFont"))
			]
			
			// Bills Tracked
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(FMargin(0.0f, 0.0f, 12.0f, 0.0f))
			[
				SAssignNew(BillsLabel, STextBlock)
				.Text(this, &SUnionPulse::GetBillsText)
				.Font(FAppStyle::GetFontStyle("SmallFont"))
			]
			
			// Contracts Uploaded
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(FMargin(0.0f, 0.0f, 12.0f, 0.0f))
			[
				SAssignNew(ContractsLabel, STextBlock)
				.Text(this, &SUnionPulse::GetContractsText)
				.Font(FAppStyle::GetFontStyle("SmallFont"))
			]
			
			// Token Balance
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(TokensLabel, STextBlock)
				.Text(this, &SUnionPulse::GetTokensText)
				.Font(FAppStyle::GetFontStyle("SmallFont"))
			]
		]
	];

	// Subscribe to store events
	OnPresenceHandle = Store->OnPresenceUpdated().AddRaw(this, &SUnionPulse::OnPresenceUpdated);
	OnBillsHandle = Store->OnBillsUpdated().AddRaw(this, &SUnionPulse::OnBillsUpdated);
	OnContractsHandle = Store->OnContractsUpdated().AddRaw(this, &SUnionPulse::OnContractsUpdated);
	OnTokensHandle = Store->OnTokensUpdated().AddRaw(this, &SUnionPulse::OnTokensUpdated);
	OnTierHandle = Store->OnTierChanged().AddRaw(this, &SUnionPulse::OnTierChanged);
}

SUnionPulse::~SUnionPulse()
{
	if (Store)
	{
		Store->OnPresenceUpdated().Remove(OnPresenceHandle);
		Store->OnBillsUpdated().Remove(OnBillsHandle);
		Store->OnContractsUpdated().Remove(OnContractsHandle);
		Store->OnTokensUpdated().Remove(OnTokensHandle);
		Store->OnTierChanged().Remove(OnTierHandle);
	}
}

void SUnionPulse::OnPresenceUpdated()
{
	if (PresenceLabel.IsValid())
	{
		PresenceLabel->SetText(GetPresenceText());
	}
}

void SUnionPulse::OnBillsUpdated()
{
	if (BillsLabel.IsValid())
	{
		BillsLabel->SetText(GetBillsText());
	}
}

void SUnionPulse::OnContractsUpdated()
{
	if (ContractsLabel.IsValid())
	{
		ContractsLabel->SetText(GetContractsText());
	}
}

void SUnionPulse::OnTokensUpdated()
{
	if (TokensLabel.IsValid())
	{
		TokensLabel->SetText(GetTokensText());
	}
}

void SUnionPulse::OnTierChanged()
{
	if (TierBadge.IsValid())
	{
		TierBadge->SetText(GetTierText());
	}
}

FText SUnionPulse::GetTierText() const
{
	return FText::FromString(Store->GetTierLabel());
}

FText SUnionPulse::GetPresenceText() const
{
	return FText::Format(NSLOCTEXT("UnionPulse", "Online", "🟢 {0} Online"),
		FText::AsNumber(Store->GetOnlineCount()));
}

FText SUnionPulse::GetBillsText() const
{
	return FText::Format(NSLOCTEXT("UnionPulse", "Bills", "📋 {0} Bills"),
		FText::AsNumber(Store->GetBillsTracked()));
}

FText SUnionPulse::GetContractsText() const
{
	return FText::Format(NSLOCTEXT("UnionPulse", "Contracts", "📄 {0} Contracts"),
		FText::AsNumber(Store->GetContractsUploaded()));
}

FText SUnionPulse::GetTokensText() const
{
	return FText::Format(NSLOCTEXT("UnionPulse", "Tokens", "⚡ {0} Tokens"),
		FText::AsNumber(Store->GetTokenBalance()));
}

FSlateColor SUnionPulse::GetTierBadgeColor() const
{
	FString Tier = Store->GetTierLabel();
	if (Tier == TEXT("Enterprise"))
		return FLinearColor::Yellow;
	if (Tier == TEXT("Pro"))
		return FLinearColor::Cyan;
	if (Tier == TEXT("Starter"))
		return FLinearColor::Green;
	return FLinearColor::Gray;
}

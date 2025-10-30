#include "SExportMenu.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"

void SExportMenu::Construct(const FArguments& InArgs)
{
	CaseId = InArgs._CaseId;
	TenantId = InArgs._TenantId;

	ChildSlot
	[
		BuildMenu()
	];
}

TSharedRef<SWidget> SExportMenu::BuildMenu()
{
	return SNew(SBorder)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(8.0f)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Export Case Agent Documents")))
		]
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(8.0f)
		[
			BuildMenuItems()
		]
	];
}

TSharedRef<SWidget> SExportMenu::BuildMenuItems()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	const TArray<FDocumentSpec>& AllDocs = FDocumentRegistry::GetAll();

	for (const FDocumentSpec& Spec : AllDocs)
	{
		// Build button label: "DisplayName (Format, ~XXXkb)"
		const FString ButtonLabel = FString::Printf(
			TEXT("%s (%s, ~%dkb)"),
			*Spec.DisplayName,
			*Spec.Format,
			Spec.EstimatedSizeKb
		);

		VBox->AddSlot().AutoHeight().Padding(4.0f)
		[
			SNew(SButton)
			.Text(FText::FromString(ButtonLabel))
			.OnClicked_Lambda([this, Spec]() { return OnExportClicked(Spec.Type); })
		];
	}

	return VBox;
}

FReply SExportMenu::OnExportClicked(EDocumentType Type)
{
	const FDocumentSpec* Spec = FDocumentRegistry::Find(Type);
	if (!Spec)
	{
		// Type not in registry; should never happen, but fail safe
		return FReply::Handled();
	}

	if (Spec->bRequiresApproval)
	{
		// Show approval dialog before proceeding
		PendingExportType = Type;
		bShowingApprovalDialog = true;
		// TODO: Show modal dialog with CaseId, DocumentType, and approval buttons
		// On confirmed, call OnExportConfirmed(Type)
	}
	else
	{
		// Direct export
		OnExportConfirmed(Type);
	}

	return FReply::Handled();
}

void SExportMenu::OnExportConfirmed(EDocumentType Type)
{
	// Trigger export via delegate
	OnExportRequested.ExecuteIfBound(Type);

	// TODO: Call ApiClient::ExportDocument(CaseId, Type)
	// Once complete, show success toast + hash verification
}

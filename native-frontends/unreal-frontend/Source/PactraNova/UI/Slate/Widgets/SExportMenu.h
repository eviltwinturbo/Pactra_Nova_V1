#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Services/DocumentRegistry.h"

/**
 * SExportMenu
 * 
 * Slate widget for exporting Case Agent documents.
 * Reads from FDocumentRegistry to populate menu items.
 * Enforces capability honesty: only offers what's in registry.
 * Shows approval gates for high-value exports (EvidencePack, CounterProposal).
 */
class SExportMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SExportMenu) {}
		SLATE_ARGUMENT(FString, CaseId)
		SLATE_ARGUMENT(FString, TenantId)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FString CaseId;
	FString TenantId;

	// Menu building
	TSharedRef<SWidget> BuildMenu();
	TSharedRef<SWidget> BuildMenuItems();

	// Export handlers
	FReply OnExportClicked(EDocumentType Type);
	void OnExportConfirmed(EDocumentType Type);

	// UI state
	EDocumentType PendingExportType = EDocumentType::MAX;
	bool bShowingApprovalDialog = false;

	// Delegates
	DECLARE_DELEGATE_OneParam(FOnExportRequested, EDocumentType);
	FOnExportRequested OnExportRequested;
};

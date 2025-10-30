#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"

/**
 * EDocumentType
 * 
 * Canonical list of document types Case Agent can generate.
 * If a type is NOT in this enum, Case Agent must NOT offer it.
 * Capability honesty: only what's in registry, nothing more.
 */
enum class EDocumentType : uint8
{
	// Briefings & Summaries
	BriefingCard,           // Negotiation Briefing Card (Markdown/PDF)
	DailyDigest,            // Daily Digest (Markdown/Email)
	ExecutiveSummary,       // Executive Summary (DOCX/PDF)

	// Evidence & Compliance
	EvidencePack,           // Evidence Pack (ZIP + SHA256 manifest)
	AuditReport,            // Audit Report (PDF/CSV)
	ComplianceChecklist,    // Compliance Checklist (Markdown/CSV)

	// Contracts & Proposals
	ClauseComparison,       // Clause Comparison Report (DOCX/PDF)
	CounterProposal,        // Counter-Proposal Draft (DOCX/Markdown)
	ProposalIntelligence,   // Proposal Intelligence Report (PDF)

	// Legislation & Policy
	BillTracking,           // Bill Tracking Report (CSV/PDF)
	LegislationDigest,      // Legislation Digest (Markdown)

	// Analytics & Metrics
	TokenUsageChart,        // Token Usage Chart (PNG/CSV)
	PerformanceReport,      // Performance Report (CSV/Markdown)
	UnionPulseSnapshot,     // Union Pulse Snapshot (PDF)

	MAX
};

/**
 * FDocumentSpec
 * 
 * Specification for a supported document type.
 * Defines display name, output format, approval requirements, and stress‑test hints.
 */
struct FDocumentSpec
{
	EDocumentType Type;
	FString DisplayName;           // e.g., "Negotiation Briefing Card"
	FString Format;                // e.g., "PDF", "CSV", "DOCX", "ZIP"
	bool bRequiresApproval;        // If true, export needs explicit user confirm
	bool bSupportsMultiTenant;     // If true, can be exported for >1 tenant in batch
	int32 EstimatedSizeKb;         // Rough size estimate (for UX pre‑warnings)
	TArray<FString> PrerequisiteData; // e.g., { "Clauses", "Bills", "Timeline" }
};

/**
 * FDocumentRegistry
 * 
 * Canonical, immutable registry of all document types Case Agent supports.
 * Enforces capability honesty: no type → no export offer → no false promises.
 * 
 * Usage:
 *   const FDocumentSpec* Spec = FDocumentRegistry::Find(EDocumentType::EvidencePack);
 *   if (Spec && Spec->bRequiresApproval) { ... show approval UI ... }
 */
class FDocumentRegistry
{
public:
	// Get all supported document types
	static const TArray<FDocumentSpec>& GetAll();

	// Find spec by type (returns nullptr if unsupported)
	static const FDocumentSpec* Find(EDocumentType Type);

	// Check if type is supported
	static bool IsSupported(EDocumentType Type) { return Find(Type) != nullptr; }

	// Get count of supported types
	static int32 Count() { return GetAll().Num(); }

private:
	static void InitializeRegistry();
	static TArray<FDocumentSpec> Registry;
	static bool bInitialized;
};

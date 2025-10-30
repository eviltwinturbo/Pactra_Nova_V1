#include "DocumentRegistry.h"

TArray<FDocumentSpec> FDocumentRegistry::Registry;
bool FDocumentRegistry::bInitialized = false;

void FDocumentRegistry::InitializeRegistry()
{
	if (bInitialized) return;

	Registry = {
		// Briefings & Summaries
		{
			EDocumentType::BriefingCard,
			TEXT("Negotiation Briefing Card"),
			TEXT("PDF"),
			false,
			false,
			50,
			{ TEXT("Clauses"), TEXT("Bills"), TEXT("Timeline") }
		},
		{
			EDocumentType::DailyDigest,
			TEXT("Daily Digest"),
			TEXT("Markdown"),
			false,
			true,
			10,
			{ TEXT("Alerts"), TEXT("Timeline"), TEXT("TokenUsage") }
		},
		{
			EDocumentType::ExecutiveSummary,
			TEXT("Executive Summary"),
			TEXT("PDF"),
			false,
			false,
			25,
			{ TEXT("Clauses"), TEXT("Bills"), TEXT("Metrics") }
		},

		// Evidence & Compliance
		{
			EDocumentType::EvidencePack,
			TEXT("Evidence Pack"),
			TEXT("ZIP"),
			true,  // Requires approval before export
			false,
			500,
			{ TEXT("Clauses"), TEXT("Statutes"), TEXT("Provenance") }
		},
		{
			EDocumentType::AuditReport,
			TEXT("Audit Report"),
			TEXT("CSV"),
			false,
			true,
			20,
			{ TEXT("Timeline"), TEXT("Hashes"), TEXT("Approvals") }
		},
		{
			EDocumentType::ComplianceChecklist,
			TEXT("Compliance Checklist"),
			TEXT("Markdown"),
			false,
			false,
			15,
			{ TEXT("TierGating"), TEXT("Provenance"), TEXT("WCAG") }
		},

		// Contracts & Proposals
		{
			EDocumentType::ClauseComparison,
			TEXT("Clause Comparison Report"),
			TEXT("PDF"),
			false,
			false,
			100,
			{ TEXT("Clauses"), TEXT("Diffs"), TEXT("Highlights") }
		},
		{
			EDocumentType::CounterProposal,
			TEXT("Counter-Proposal Draft"),
			TEXT("DOCX"),
			true,  // Requires approval before generation
			false,
			150,
			{ TEXT("OriginalProposal"), TEXT("AIGenerated"), TEXT("RiskScoring") }
		},
		{
			EDocumentType::ProposalIntelligence,
			TEXT("Proposal Intelligence Report"),
			TEXT("PDF"),
			false,
			false,
			75,
			{ TEXT("Proposal"), TEXT("RiskScoring"), TEXT("MonteCarlo") }
		},

		// Legislation & Policy
		{
			EDocumentType::BillTracking,
			TEXT("Bill Tracking Report"),
			TEXT("CSV"),
			false,
			true,
			10,
			{ TEXT("TrackedBills"), TEXT("Status"), TEXT("Conflicts") }
		},
		{
			EDocumentType::LegislationDigest,
			TEXT("Legislation Digest"),
			TEXT("Markdown"),
			false,
			true,
			15,
			{ TEXT("Bills"), TEXT("Timeline"), TEXT("Links") }
		},

		// Analytics & Metrics
		{
			EDocumentType::TokenUsageChart,
			TEXT("Token Usage Chart"),
			TEXT("PNG"),
			false,
			false,
			50,
			{ TEXT("TokenUsage"), TEXT("FeatureBreakdown"), TEXT("TimeSeries") }
		},
		{
			EDocumentType::PerformanceReport,
			TEXT("Performance Report"),
			TEXT("CSV"),
			false,
			true,
			20,
			{ TEXT("Latency"), TEXT("Throughput"), TEXT("ErrorRates") }
		},
		{
			EDocumentType::UnionPulseSnapshot,
			TEXT("Union Pulse Snapshot"),
			TEXT("PDF"),
			false,
			false,
			30,
			{ TEXT("Metrics"), TEXT("Trends"), TEXT("Timestamp") }
		}
	};

	bInitialized = true;
}

const TArray<FDocumentSpec>& FDocumentRegistry::GetAll()
{
	if (!bInitialized)
	{
		InitializeRegistry();
	}
	return Registry;
}

const FDocumentSpec* FDocumentRegistry::Find(EDocumentType Type)
{
	if (!bInitialized)
	{
		InitializeRegistry();
	}

	for (const FDocumentSpec& Spec : Registry)
	{
		if (Spec.Type == Type)
		{
			return &Spec;
		}
	}

	return nullptr;
}

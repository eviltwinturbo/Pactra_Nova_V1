using System.Collections.Generic;
using System.Linq;

namespace PactraNova.UI.Services
{
    /// <summary>
    /// EDocumentType
    /// 
    /// Canonical list of document types Case Agent can generate.
    /// If a type is NOT in this enum, Case Agent must NOT offer it.
    /// Capability honesty: only what's in registry, nothing more.
    /// </summary>
    public enum DocumentType
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
        UnionPulseSnapshot      // Union Pulse Snapshot (PDF)
    }

    /// <summary>
    /// DocumentSpec
    /// 
    /// Specification for a supported document type.
    /// Defines display name, output format, approval requirements, and stress-test hints.
    /// </summary>
    public class DocumentSpec
    {
        public DocumentType Type { get; set; }
        public string DisplayName { get; set; }
        public string Format { get; set; }
        public bool RequiresApproval { get; set; }
        public bool SupportsMultiTenant { get; set; }
        public int EstimatedSizeKb { get; set; }
        public List<string> PrerequisiteData { get; set; }
    }

    /// <summary>
    /// DocumentRegistry
    /// 
    /// Canonical, immutable registry of all document types Case Agent supports.
    /// Enforces capability honesty: no type → no export offer → no false promises.
    /// 
    /// Usage:
    ///   DocumentSpec spec = DocumentRegistry.Find(DocumentType.EvidencePack);
    ///   if (spec != null && spec.RequiresApproval) { ... show approval UI ... }
    /// </summary>
    public static class DocumentRegistry
    {
        private static readonly List<DocumentSpec> Registry = new List<DocumentSpec>
        {
            // Briefings & Summaries
            new DocumentSpec
            {
                Type = DocumentType.BriefingCard,
                DisplayName = "Negotiation Briefing Card",
                Format = "PDF",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 150,
                PrerequisiteData = new List<string> { "Clauses", "Bills", "Timeline" }
            },
            new DocumentSpec
            {
                Type = DocumentType.DailyDigest,
                DisplayName = "Daily Digest",
                Format = "Markdown",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 50,
                PrerequisiteData = new List<string> { "Timeline", "Alerts" }
            },
            new DocumentSpec
            {
                Type = DocumentType.ExecutiveSummary,
                DisplayName = "Executive Summary",
                Format = "DOCX",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 200,
                PrerequisiteData = new List<string> { "Clauses", "Bills", "Metrics" }
            },

            // Evidence & Compliance
            new DocumentSpec
            {
                Type = DocumentType.EvidencePack,
                DisplayName = "Evidence Pack",
                Format = "ZIP",
                RequiresApproval = true,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 5000,
                PrerequisiteData = new List<string> { "Clauses", "Statutes", "Provenance" }
            },
            new DocumentSpec
            {
                Type = DocumentType.AuditReport,
                DisplayName = "Audit Report",
                Format = "CSV",
                RequiresApproval = false,
                SupportsMultiTenant = true,
                EstimatedSizeKb = 300,
                PrerequisiteData = new List<string> { "Timeline", "Hashes", "Approvals" }
            },
            new DocumentSpec
            {
                Type = DocumentType.ComplianceChecklist,
                DisplayName = "Compliance Checklist",
                Format = "CSV",
                RequiresApproval = false,
                SupportsMultiTenant = true,
                EstimatedSizeKb = 100,
                PrerequisiteData = new List<string> { "WCAG", "Tier Gating", "Provenance" }
            },

            // Contracts & Proposals
            new DocumentSpec
            {
                Type = DocumentType.ClauseComparison,
                DisplayName = "Clause Comparison Report",
                Format = "DOCX",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 400,
                PrerequisiteData = new List<string> { "Clauses" }
            },
            new DocumentSpec
            {
                Type = DocumentType.CounterProposal,
                DisplayName = "Counter-Proposal Draft",
                Format = "DOCX",
                RequiresApproval = true,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 500,
                PrerequisiteData = new List<string> { "Clauses", "Playbooks", "Provenance" }
            },
            new DocumentSpec
            {
                Type = DocumentType.ProposalIntelligence,
                DisplayName = "Proposal Intelligence Report",
                Format = "PDF",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 350,
                PrerequisiteData = new List<string> { "Clauses", "Risk Scores", "Monte Carlo" }
            },

            // Legislation & Policy
            new DocumentSpec
            {
                Type = DocumentType.BillTracking,
                DisplayName = "Bill Tracking Report",
                Format = "CSV",
                RequiresApproval = false,
                SupportsMultiTenant = true,
                EstimatedSizeKb = 250,
                PrerequisiteData = new List<string> { "Bills", "Status", "Sponsors" }
            },
            new DocumentSpec
            {
                Type = DocumentType.LegislationDigest,
                DisplayName = "Legislation Digest",
                Format = "Markdown",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 120,
                PrerequisiteData = new List<string> { "Bills" }
            },

            // Analytics & Metrics
            new DocumentSpec
            {
                Type = DocumentType.TokenUsageChart,
                DisplayName = "Token Usage Chart",
                Format = "PNG",
                RequiresApproval = false,
                SupportsMultiTenant = true,
                EstimatedSizeKb = 80,
                PrerequisiteData = new List<string> { "Tokens", "Features" }
            },
            new DocumentSpec
            {
                Type = DocumentType.PerformanceReport,
                DisplayName = "Performance Report",
                Format = "CSV",
                RequiresApproval = false,
                SupportsMultiTenant = true,
                EstimatedSizeKb = 150,
                PrerequisiteData = new List<string> { "Latency", "Throughput", "Errors" }
            },
            new DocumentSpec
            {
                Type = DocumentType.UnionPulseSnapshot,
                DisplayName = "Union Pulse Snapshot",
                Format = "PDF",
                RequiresApproval = false,
                SupportsMultiTenant = false,
                EstimatedSizeKb = 200,
                PrerequisiteData = new List<string> { "Presence", "Bills", "Contracts", "Tokens" }
            }
        };

        /// <summary>
        /// Get all supported document types
        /// </summary>
        public static List<DocumentSpec> GetAll()
        {
            return Registry;
        }

        /// <summary>
        /// Find spec by type (returns null if unsupported)
        /// </summary>
        public static DocumentSpec Find(DocumentType type)
        {
            return Registry.FirstOrDefault(spec => spec.Type == type);
        }

        /// <summary>
        /// Check if type is supported
        /// </summary>
        public static bool IsSupported(DocumentType type)
        {
            return Find(type) != null;
        }

        /// <summary>
        /// Get count of supported types
        /// </summary>
        public static int Count()
        {
            return Registry.Count;
        }
    }
}


using UnityEngine;
using UnityEngine.UIElements;
using PactraNova.UI.Services;
using System;

namespace PactraNova.UI.Widgets
{
    /// <summary>
    /// ExportMenu
    /// 
    /// Unity UI Toolkit widget for exporting Case Agent documents.
    /// Reads from DocumentRegistry to populate menu items.
    /// Enforces capability honesty: only offers what's in registry.
    /// Shows approval gates for high-value exports (EvidencePack, CounterProposal).
    /// </summary>
    public class ExportMenu : VisualElement
    {
        public new class UxmlFactory : UxmlFactory<ExportMenu, UxmlTraits> { }

        private string caseId;
        private string tenantId;
        private DocumentType? pendingExportType;
        private bool showingApprovalDialog;

        private VisualElement menuContainer;
        private VisualElement approvalDialog;
        private Label approvalMessage;
        private Button confirmButton;
        private Button cancelButton;

        public ExportMenu()
        {
            // Initialize UI structure
            AddToClassList("export-menu");

            // Menu container
            menuContainer = new VisualElement();
            menuContainer.AddToClassList("export-menu-container");
            Add(menuContainer);

            // Approval dialog (hidden by default)
            approvalDialog = new VisualElement();
            approvalDialog.AddToClassList("approval-dialog");
            approvalDialog.style.display = DisplayStyle.None;

            approvalMessage = new Label();
            approvalMessage.AddToClassList("approval-message");
            approvalDialog.Add(approvalMessage);

            var buttonRow = new VisualElement();
            buttonRow.AddToClassList("approval-buttons");

            confirmButton = new Button(OnApprovalConfirmed) { text = "Confirm Export" };
            confirmButton.AddToClassList("approval-confirm");
            buttonRow.Add(confirmButton);

            cancelButton = new Button(OnApprovalCancelled) { text = "Cancel" };
            cancelButton.AddToClassList("approval-cancel");
            buttonRow.Add(cancelButton);

            approvalDialog.Add(buttonRow);
            Add(approvalDialog);

            BuildMenu();
        }

        /// <summary>
        /// Set case and tenant context
        /// </summary>
        public void SetContext(string caseId, string tenantId)
        {
            this.caseId = caseId;
            this.tenantId = tenantId;
        }

        /// <summary>
        /// Build menu from DocumentRegistry
        /// </summary>
        private void BuildMenu()
        {
            menuContainer.Clear();

            var specs = DocumentRegistry.GetAll();

            // Group by category
            var categories = new System.Collections.Generic.Dictionary<string, System.Collections.Generic.List<DocumentSpec>>
            {
                { "Briefings & Summaries", new System.Collections.Generic.List<DocumentSpec>() },
                { "Evidence & Compliance", new System.Collections.Generic.List<DocumentSpec>() },
                { "Contracts & Proposals", new System.Collections.Generic.List<DocumentSpec>() },
                { "Legislation & Policy", new System.Collections.Generic.List<DocumentSpec>() },
                { "Analytics & Metrics", new System.Collections.Generic.List<DocumentSpec>() }
            };

            foreach (var spec in specs)
            {
                string category = GetCategory(spec.Type);
                if (categories.ContainsKey(category))
                {
                    categories[category].Add(spec);
                }
            }

            // Build UI for each category
            foreach (var kvp in categories)
            {
                if (kvp.Value.Count == 0) continue;

                var categoryLabel = new Label(kvp.Key);
                categoryLabel.AddToClassList("export-category-label");
                menuContainer.Add(categoryLabel);

                foreach (var spec in kvp.Value)
                {
                    var button = new Button(() => OnExportClicked(spec.Type))
                    {
                        text = $"{spec.DisplayName} ({spec.Format})"
                    };
                    button.AddToClassList("export-button");

                    if (spec.RequiresApproval)
                    {
                        button.AddToClassList("export-button-approval-required");
                    }

                    menuContainer.Add(button);
                }
            }
        }

        /// <summary>
        /// Get category for document type
        /// </summary>
        private string GetCategory(DocumentType type)
        {
            switch (type)
            {
                case DocumentType.BriefingCard:
                case DocumentType.DailyDigest:
                case DocumentType.ExecutiveSummary:
                    return "Briefings & Summaries";

                case DocumentType.EvidencePack:
                case DocumentType.AuditReport:
                case DocumentType.ComplianceChecklist:
                    return "Evidence & Compliance";

                case DocumentType.ClauseComparison:
                case DocumentType.CounterProposal:
                case DocumentType.ProposalIntelligence:
                    return "Contracts & Proposals";

                case DocumentType.BillTracking:
                case DocumentType.LegislationDigest:
                    return "Legislation & Policy";

                case DocumentType.TokenUsageChart:
                case DocumentType.PerformanceReport:
                case DocumentType.UnionPulseSnapshot:
                    return "Analytics & Metrics";

                default:
                    return "Other";
            }
        }

        /// <summary>
        /// Handle export button click
        /// </summary>
        private void OnExportClicked(DocumentType type)
        {
            var spec = DocumentRegistry.Find(type);
            if (spec == null)
            {
                Debug.LogError($"[ExportMenu] Document type {type} not found in registry");
                return;
            }

            if (spec.RequiresApproval)
            {
                // Show approval dialog
                pendingExportType = type;
                showingApprovalDialog = true;

                approvalMessage.text = $"Export {spec.DisplayName}?\n\n" +
                    $"This document contains sensitive data and requires explicit approval.\n" +
                    $"Format: {spec.Format}\n" +
                    $"Estimated Size: ~{spec.EstimatedSizeKb} KB\n\n" +
                    $"A SHA256 provenance hash will be generated and logged.";

                menuContainer.style.display = DisplayStyle.None;
                approvalDialog.style.display = DisplayStyle.Flex;
            }
            else
            {
                // Export directly
                ExecuteExport(type);
            }
        }

        /// <summary>
        /// Handle approval confirmation
        /// </summary>
        private void OnApprovalConfirmed()
        {
            if (pendingExportType.HasValue)
            {
                ExecuteExport(pendingExportType.Value);
            }

            ResetApprovalDialog();
        }

        /// <summary>
        /// Handle approval cancellation
        /// </summary>
        private void OnApprovalCancelled()
        {
            ResetApprovalDialog();
        }

        /// <summary>
        /// Reset approval dialog state
        /// </summary>
        private void ResetApprovalDialog()
        {
            pendingExportType = null;
            showingApprovalDialog = false;
            menuContainer.style.display = DisplayStyle.Flex;
            approvalDialog.style.display = DisplayStyle.None;
        }

        /// <summary>
        /// Execute document export
        /// </summary>
        private void ExecuteExport(DocumentType type)
        {
            var spec = DocumentRegistry.Find(type);
            if (spec == null)
            {
                Debug.LogError($"[ExportMenu] Document type {type} not found in registry");
                return;
            }

            Debug.Log($"[ExportMenu] Exporting {spec.DisplayName} (Format: {spec.Format})");

            // TODO: Wire to ApiClient.ExportDocument(type, caseId, tenantId)
            // This will:
            // 1. Call backend endpoint /v1/case-agent/export
            // 2. Generate SHA256 hash
            // 3. Log to CHANGELOG
            // 4. Return file path or download URL

            // Placeholder: simulate export
            var apiClient = UnityEngine.Object.FindObjectOfType<ApiClient>();
            if (apiClient != null)
            {
                apiClient.ExportDocument(type, caseId, tenantId, OnExportSuccess, OnExportError);
            }
            else
            {
                Debug.LogError("[ExportMenu] ApiClient not found in scene");
            }
        }

        /// <summary>
        /// Handle export success
        /// </summary>
        private void OnExportSuccess(string filePath, string sha256Hash)
        {
            Debug.Log($"[ExportMenu] Export successful: {filePath} (SHA256: {sha256Hash})");

            // TODO: Show success toast
            // TODO: Offer to open file or copy hash
        }

        /// <summary>
        /// Handle export error
        /// </summary>
        private void OnExportError(string error)
        {
            Debug.LogError($"[ExportMenu] Export failed: {error}");

            // TODO: Show error toast
        }
    }
}


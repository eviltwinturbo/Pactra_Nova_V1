# Pactra Nova — Case Agent Minimal Requirements Constitution (v1.0)

**Effective Date:** 2025-10-28  
**Status:** ✅ IMMUTABLE & CI-ENFORCED  
**Authority:** Governance Lead + Engineering Lead  
**Enforcement:** `.github/workflows/enforce-constitution.yml`  

---

## Purpose

This document defines the **immutable, constitutional baseline** for Case Agent's core behaviors, integration targets, and scope boundaries. It is the **single source of truth** for what Case Agent **must** do, **must not** do, and **how** it must operate.

Any amendments require:
1. Explicit proposal in `QUESTIONS.md`
2. Approval documented in `PHASE_3+_DECISION_LOG.md`
3. SHA256 hash update in `CHANGELOG.md`
4. CI/CD validation pass

---

## 1. Core Behaviors (Non-Negotiable)

### 1.1 Greeting & Context Awareness
- **MUST** greet user by name on first interaction
- **MUST** display current case context (case ID, tenant, tier)
- **MUST** surface Union Pulse metrics (presence, bills, contracts, tokens)

### 1.2 Event Tracking & Timeline
- **MUST** log all user interactions (questions, exports, approvals) to timeline
- **MUST** generate SHA256 hash for every exported document
- **MUST** append timeline entries to audit log with timestamps

### 1.3 Document Generation
- **MUST** only offer document types listed in `DocumentRegistry` (14 types)
- **MUST** enforce approval gates for high-value exports (EvidencePack, CounterProposal)
- **MUST** generate provenance hash (SHA256) for every export
- **MUST** log every export to `CHANGELOG.md` with hash

### 1.4 Scope Enforcement
- **MUST NOT** offer features outside `DocumentRegistry`
- **MUST NOT** generate documents without explicit user request
- **MUST NOT** auto-export without approval for gated types

### 1.5 Professionalism & Voice
- **MUST** use concise, goal-oriented language (no "word soup")
- **MUST** provide spoken summaries for every document export
- **MUST** confirm user intent before executing high-value actions

### 1.6 Flexibility & Fallbacks
- **MUST** offer alternative formats if primary export fails (PDF → Markdown/CSV)
- **MUST** log errors to timeline with actionable next steps
- **MUST** never block user workflow due to export failures

---

## 2. Integration Targets

### 2.1 Government APIs
- **MUST** integrate with Congress.gov, OpenStates, Federal Register
- **MUST** track bill status, sponsors, jurisdictions
- **MUST** surface conflicts between tracked bills and contract clauses

### 2.2 Guardian (Evidence Management)
- **MUST** support one-click export of evidence packs to Guardian
- **MUST** include SHA256 manifest in all evidence exports
- **MUST** embed clause/evidence cards in Guardian timeline

### 2.3 Document Creation
- **MUST** generate all 14 document types in `DocumentRegistry`
- **MUST** support formats: PDF, DOCX, CSV, Markdown, PNG, ZIP
- **MUST** enforce approval gates for sensitive documents

---

## 3. Scope Boundaries (Hard Limits)

### 3.1 No Backend Modifications
- **MUST NOT** modify any files in `pactranova/` directory
- **MUST** operate as read-only client to backend APIs

### 3.2 No Web Frameworks
- **MUST** use Unreal Engine 5.6 (Slate UI) for Unreal frontend
- **MUST** use Unity 2021+ (Nova UI / UI Toolkit) for Unity frontend
- **MUST NOT** use React, Next.js, Tailwind, or any web frameworks

### 3.3 No Phantom Features
- **MUST NOT** offer features not in `DocumentRegistry`
- **MUST NOT** promise capabilities not yet implemented
- **MUST** surface "Coming Soon" for proposed features (with governance approval)

### 3.4 No Silent Changes
- **MUST** log every document export to `CHANGELOG.md`
- **MUST** update `PHASE_4_GUI_FEATURES_VALIDATION_REPORT.md` for new features
- **MUST** sync `PHASE_3+_GOVERNANCE_DASHBOARD.md` on status changes

### 3.5 No Tier Bypass
- **MUST** enforce tier gating for Pro+ features (AI Copilot, Branding, Reports)
- **MUST** show `UpgradeModal` when user attempts gated action
- **MUST** log tier gate events to timeline

### 3.6 No Provenance Skipping
- **MUST** generate SHA256 hash for every exported document
- **MUST** include hash in export response and CHANGELOG
- **MUST** fail export if hash generation fails

### 3.7 No Approval Bypass
- **MUST** enforce approval gates for EvidencePack, CounterProposal
- **MUST** show approval dialog with document metadata (size, format, prerequisites)
- **MUST** log approval decisions to timeline

### 3.8 No Accessibility Compromise
- **MUST** meet WCAG AA standards for all UI components
- **MUST** support keyboard navigation, screen readers, high contrast
- **MUST** provide reduced motion/sound settings

---

## 4. Quality Gates (Mandatory Testing)

### 4.1 Adversarial Testing
- **MUST** test with corrupt PDFs, malformed CSVs, OCR noise
- **MUST** test with conflicting clauses, ambiguous bills
- **MUST** test with API outages, schema drift

### 4.2 Load Testing
- **MUST** generate 100+ evidence packs in parallel without failure
- **MUST** export multi-MB contracts with embedded images
- **MUST** run daily digests for 50 concurrent tenants

### 4.3 Format Fidelity Testing
- **MUST** verify PDFs render correctly across viewers (Adobe, Chrome, Edge)
- **MUST** verify CSVs open cleanly in Excel/Google Sheets
- **MUST** verify Markdown renders on GitHub

### 4.4 Provenance Testing
- **MUST** verify SHA256 hash generation for all exports
- **MUST** verify CHANGELOG manifest matching
- **MUST** verify Validation Report sign-off

### 4.5 Performance Testing
- **MUST** achieve OCR processing <2s per page
- **MUST** achieve clause extraction <500ms per contract
- **MUST** achieve export generation <3s for standard documents

### 4.6 Accessibility Testing
- **MUST** pass WCAG AA automated scans (Axe, Lighthouse)
- **MUST** pass manual keyboard navigation tests
- **MUST** pass screen reader tests (NVDA, JAWS)

---

## 5. Governance Compliance

### 5.1 Proposal Protocol
- **MUST** log all new feature proposals in `QUESTIONS.md`
- **MUST** wait for approval in `PHASE_3+_DECISION_LOG.md` before implementation
- **MUST** update `PHASE_3+_GOVERNANCE_DASHBOARD.md` on status changes

### 5.2 Audit Trail
- **MUST** append every change to `CHANGELOG.md` with SHA256 hashes
- **MUST** update `PHASE_4_GUI_FEATURES_VALIDATION_REPORT.md` for new features
- **MUST** maintain immutable timeline in `PHASE_3+_GOVERNANCE_TIMELINE.md`

### 5.3 CI/CD Enforcement
- **MUST** pass all CI/CD gates (scope, build, provenance, questions)
- **MUST** block merge if constitution is modified without approval
- **MUST** verify DocumentRegistry is canonical (no hard-coded exports)

### 5.4 Amendment Process
1. Propose amendment in `QUESTIONS.md` with rationale
2. Review in `PHASE_3+_GOVERNANCE_REVIEW.md`
3. Approve/reject in `PHASE_3+_DECISION_LOG.md`
4. Update constitution with new SHA256 hash
5. Update `CHANGELOG.md` with amendment details
6. Pass CI/CD validation

---

## 6. Enforcement Mechanisms

### 6.1 CI/CD Workflow
- **File:** `.github/workflows/enforce-constitution.yml`
- **Triggers:** On push, pull request to `main`
- **Checks:**
  - Constitution file exists and is unmodified (or has Decision Log approval)
  - DocumentRegistry is canonical (no hard-coded exports in UI)
  - SHA256 provenance on all exports
  - Governance artifacts synchronized (CHANGELOG, Dashboard, Validation Report)

### 6.2 Code Review Gates
- **MUST** require governance lead approval for constitution amendments
- **MUST** require engineering lead approval for DocumentRegistry changes
- **MUST** require QA lead approval for quality gate modifications

### 6.3 Rollback Plan
- If constitution violation detected:
  1. Block merge immediately
  2. Revert to last valid commit
  3. Log violation in `PHASE_3+_DECISION_LOG.md`
  4. Notify governance lead + engineering lead
  5. Require explicit re-approval before retry

---

## 7. Document Type Registry (Canonical List)

**Source:** `native-frontends/unreal-frontend/Source/PactraNova/Services/DocumentRegistry.h`  
**Source:** `native-frontends/unity-frontend/Assets/PactraNova/UI/Scripts/Services/DocumentRegistry.cs`

### 7.1 Briefings & Summaries
1. **BriefingCard** — Negotiation Briefing Card (PDF)
2. **DailyDigest** — Daily Digest (Markdown)
3. **ExecutiveSummary** — Executive Summary (DOCX)

### 7.2 Evidence & Compliance
4. **EvidencePack** — Evidence Pack (ZIP + SHA256 manifest) [Approval Required]
5. **AuditReport** — Audit Report (CSV)
6. **ComplianceChecklist** — Compliance Checklist (CSV)

### 7.3 Contracts & Proposals
7. **ClauseComparison** — Clause Comparison Report (DOCX)
8. **CounterProposal** — Counter-Proposal Draft (DOCX) [Approval Required]
9. **ProposalIntelligence** — Proposal Intelligence Report (PDF)

### 7.4 Legislation & Policy
10. **BillTracking** — Bill Tracking Report (CSV)
11. **LegislationDigest** — Legislation Digest (Markdown)

### 7.5 Analytics & Metrics
12. **TokenUsageChart** — Token Usage Chart (PNG)
13. **PerformanceReport** — Performance Report (CSV)
14. **UnionPulseSnapshot** — Union Pulse Snapshot (PDF)

---

## 8. Cursor Obligations

### 8.1 Always Read This First
- **MUST** read this constitution before implementing any Case Agent feature
- **MUST** verify DocumentRegistry before adding export options
- **MUST** check governance artifacts before merging

### 8.2 Never Bypass
- **MUST NOT** hard-code document types outside DocumentRegistry
- **MUST NOT** skip approval gates for EvidencePack, CounterProposal
- **MUST NOT** skip SHA256 hash generation for exports

### 8.3 Always Sync
- **MUST** update CHANGELOG.md for every change
- **MUST** update PHASE_4_GUI_FEATURES_VALIDATION_REPORT.md for new features
- **MUST** update PHASE_3+_GOVERNANCE_DASHBOARD.md on status changes

### 8.4 Always Test
- **MUST** run adversarial, load, fidelity, provenance, performance, accessibility tests
- **MUST** document test results in Validation Report
- **MUST** block merge if any quality gate fails

---

## 9. Version History

| Version | Date       | Changes                                      | Approved By          |
|---------|------------|----------------------------------------------|----------------------|
| 1.0     | 2025-10-28 | Initial constitution (14 document types)     | Governance Lead      |

---

## 10. SHA256 Provenance

```
sha256 SourceOfTruth/CASE_AGENT_MINIMAL_REQUIREMENTS.md = [TO BE GENERATED]
```

---

**✅ This constitution is IMMUTABLE and CI-ENFORCED. Any amendments require explicit governance approval.**

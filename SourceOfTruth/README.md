# Pactra Nova — Source of Truth Governance

**Purpose:**  
This folder (`SourceOfTruth/`) is the **only canonical location** for Pactra Nova configuration, tier data, and authoritative specifications.  
All other documents, specs, and UI code must reference these files — never duplicate values.

---

## Day 0 Frontend Reset Constitution

**Effective Date:** 2025-10-28  
**Status:** ✅ ACTIVE & ENFORCED

### Frontend Scope (Unreal + Unity Only)

The **only** authorized frontend frameworks and UIs are:

**Permitted Frameworks:**
- Unreal Engine 5.6 with Slate UI
- Unity 2021+ with Nova UI or UI Toolkit

**Permitted UIs:**
- Orbit Messenger (`/orbit/*`)
- Case Agent (`/case-agent/*`)

**Forbidden:**
- All web frameworks (React, Next.js, Vue, Angular, etc.)
- Marketing pages, pricing UIs, dashboards (unless explicitly approved)
- Any frontend code outside `native-frontends/` directory

### Rules for Cursor and All Contributors

1. **Always Read First**
   - Before creating any file, read `FRONTEND_DIRECTIVES.md` and this section.
   - If instructions conflict, this README wins.

2. **Backend is Untouchable**
   - The `pactranova/` directory is **completely off-limits**.
   - Never modify, delete, or restructure backend files.

3. **Framework Lock**
   - You are **forbidden** from using web frameworks.
   - Only Unreal Slate and Unity Nova/UI Toolkit are allowed.

4. **Day 0 Baseline**
   - Only Orbit Messenger and Case Agent UIs are required.
   - No other UI expansions allowed without governance approval.

5. **Proposal Protocol**
   - New UI proposals must be logged in `QUESTIONS.md` with status "Proposed".
   - Proposals must be approved in this README before implementation.

### Governance Enforcement (CI/CD)

**Automated checks fail the build if:**
- Any frontend file exists outside `native-frontends/` directory
- `pactranova/` (backend) has been modified
- `QUESTIONS.md` contains unresolved "Status: Proposed" entries
- Unauthorized web framework code is detected
- Provenance hashes don't match baseline

### Process for Frontend Expansion

If a new UI is needed:

1. **Log proposal** in `QUESTIONS.md`:
   - Date, proposed path, reason, impact
   - Status: `Proposed`

2. **Governance review** → Decision makers approve/reject

3. **Update this README** with explicit approval section (if approved)

4. **Update QUESTIONS.md** status to `Approved`

5. **Then implement** — CI/CD will allow the new files

### Approved Frontend Expansions

#### 1. Realtime WebSocket Support (Orbit Messenger)

**Effective Date:** 2025-10-28  
**Status:** ✅ APPROVED  
**Scope:** Orbit Messenger only  

**Authorized Frameworks:**
- Unreal Slate UI (UE 5.6)
- Unity Nova/UI Toolkit (Unity 2021+)

**Approved Artifacts:**
- Unreal: `FWebSocketClient` (Services/) integrated with `SMessagePane` and `FApiClient`
- Unity: `WebSocketClient.cs` (Services/) integrated with `OrbitController` and `ApiClient.cs`

**Governance Constraints:**
- ✅ Backend completely untouched; no schema or service changes
- ✅ Case Agent excluded until separate proposal approved
- ✅ All changes must be hashed and logged in CHANGELOG.md
- ✅ CI/CD must pass Unreal + Unity websocket tests before merge
- ✅ Framework lock respected (Slate + Nova only)

**Exclusions (Case Agent realtime requires new QUESTIONS.md proposal)**

---

#### 2. Phase 4 GUI Features Execution (Orbit Messenger + Case Agent Enhancements)

**Effective Date:** 2025-10-28  
**Status:** 🚀 **AUTHORIZED & IN PROGRESS**  
**Scope:** Union Pulse ecosystem, Orbit Messenger enhancements, Contracts UI, Legislation UI, Reports/Guardian  

**Approved Frameworks:**
- Unreal 5.6 Slate UI
- Unity 2021+ Nova/UI Toolkit

**Approved Features (25 Total):**

**Core Ecosystem (10):**
1. Union Pulse Header Widget (metrics: presence, bills, contracts, tokens)
2. Upgrade Modal (tier-gated feature CTA)
3. Toast Notifications (success/warning/error/info)
4. Trial Banner Enhancements (days left, usage counts)
5. Presence Indicator Avatars (Green/Amber/Red/Gray)
6. Token Balance Animation (smooth count transitions)
7. Quick Actions Dropdown (New Message, Upload, Track Bill, Ask Copilot)
8. Error Banner (global API error display)
9. Loading Skeletons (shimmer placeholders)
10. Accessibility Hooks (Tab/Shift+Tab, ARIA-like labels)

**Orbit Messenger (5):**
11. Threaded Replies UI
12. Emoji Reactions Bar
13. Typing Indicator Row
14. Read Receipts
15. File Attachment Preview

**Contracts (4):**
16. Clause Category Accordion
17. Comparison View Split Screen
18. Upload Progress Bar
19. Search Within Contract

**Legislation (3):**
20. Tracked Bills Sidebar Widget
21. Bill Status Badges (color-coded)
22. Legislation Filters (sponsor, chamber, jurisdiction)

**Reports & Guardian (3):**
23. Token Usage Chart
24. Provenance Hash Viewer
25. Audit Log Timeline

**Governance Constraints:**
- ✅ Framework lock enforced (Unreal Slate + Unity Nova/UI Toolkit only)
- ✅ Backend untouched (read-only integration only)
- ✅ Tier gating map enforced (Trial/Starter/Pro/Enterprise feature access)
- ✅ All changes hashed and logged in CHANGELOG.md
- ✅ CI/CD validation passes (scope, build-unreal, build-unity, provenance)
- ✅ Accessibility compliance verified (focus order, contrast, labels)

**Documentation:**
- Execution plan: `PHASE_4_GUI_FEATURES_EXECUTION.md`
- Validation report: `PHASE_4_GUI_FEATURES_VALIDATION_REPORT.md`
- Decision entry: `PHASE_3+_DECISION_LOG.md` (Phase 4 authorization)

---

## Canonical Files

| File | Purpose | Canonical? | Editable? |
|------|---------|-----------|-----------|
| `FRONTEND_DIRECTIVES.md` | Cursor-facing frontend rules | ✅ Yes | ✅ Governance changes only |
| `FRONTEND_SKELETON.md` | Unreal + Unity skeleton specs | ✅ Yes | ✅ Structure updates only |
| `CHANGELOG.md` | Immutable change audit trail | ✅ Yes | ✅ Append-only |
| `QUESTIONS.md` | Proposed UI expansions | ✅ Yes | ✅ New proposals + approvals |

---

## Pre-Commit Checklist

Before pushing any change:

- [ ] I have read this README and FRONTEND_DIRECTIVES.md
- [ ] I have not modified `pactranova/` (backend)
- [ ] All frontend files are in `native-frontends/` (Unreal or Unity only)
- [ ] I used only Slate UI or Nova/UI Toolkit (no web frameworks)
- [ ] I updated CHANGELOG.md with provenance hashes
- [ ] I logged new UI proposals in QUESTIONS.md before implementing
- [ ] CI passes: scope enforcement + Unreal + Unity builds

---

## Anti-Patterns (Never Do This)

❌ **Do NOT create web frontend files outside `archive/`**  
❌ **Do NOT modify backend code in `pactranova/`**  
❌ **Do NOT use React, Next.js, Tailwind, or web frameworks**  
❌ **Do NOT implement new UI without governance approval**  
❌ **Do NOT bypass CI enforcement checks**  

---

## CI/CD Enforcement Scripts

The following checks run on every commit:

```bash
./scripts/enforce-frontend-scope.sh
# ✅ Verifies only native-frontends/ contains frontend code

./scripts/enforce-questions.sh
# ✅ Verifies all Proposed items have governance approvals

./scripts/hash-assets.sh
# ✅ Generates SHA256 provenance hashes for audit trail
```

If any fails, the build is **rejected**.

---

### Governance Dashboard

A consolidated governance dashboard is maintained in `PHASE_3+_GOVERNANCE_DASHBOARD.md`.  
It provides a single‑page executive snapshot of proposals, approvals, and enforcement status.

### Pending Frontend Expansions (Phase 3+)

The following proposals are logged in `QUESTIONS.md` and surfaced in `PHASE_3+_GOVERNANCE_REVIEW.md`:

- Case Agent Realtime Support
- Presence Indicators
- Typing Indicators
- Read Receipts
- Message Reactions
- Thread Support
- File Sharing

**Status:** All Proposed (awaiting governance review)  
**Constraint:** None may be implemented until explicitly approved here.

### Governance Decision Log

All final governance decisions for Phase 3+ proposals are recorded in `PHASE_3+_DECISION_LOG.md`.  
This file is immutable and serves as the canonical audit trail of approvals, rejections, or deferrals.

---

**Last Updated:** 2025-10-29  
**Governance Version:** 3.2 (Dashboard + Governance Infrastructure)  
**Status:** ✅ ACTIVE & ENFORCED

---

## 🚀 Cursor's Constitutional Discipline (Self-Healing Queue)

**Effective:** 2025-10-28

Cursor commits to the following discipline to ensure execution survives restarts:

1. **TODO_LEDGER.md is the canonical queue**
   - Located in `SourceOfTruth/TODO_LEDGER.md`
   - Updated after every action (mark items `[x]` when complete)
   - Re-hydrated on restart if ephemeral memory is lost

2. **Every todo item maps to an artifact**
   - Execution plans (`PHASE_4_TRANCHE_XXXX_EXECUTION.md`)
   - Validation reports (evidence + test counts + screenshots)
   - Governance documents (QUESTIONS.md, DECISION_LOG.md, CHANGELOG.md, GOVERNANCE_DASHBOARD.md)
   - No orphaned todos outside these artifacts

3. **Merge discipline**
   - Never merge without all acceptance gates green
   - Never merge without all todo boxes checked for the tranche in progress
   - All boxes must align to surfaced governance artifacts

4. **Immutability & Expansion**
   - Never edit prior entries in this ledger (append-only)
   - Expand forward automatically as new tranches are authorized
   - Maintain constitutional bindings (Day 0 Reset Constitution enforced)

**This discipline ensures:**
- ✅ Execution continuity across PC restarts
- ✅ Immutable audit trail (ledger + artifacts + git history)
- ✅ Constitutional governance (no drift from Day 0 Constitution)
- ✅ Self-healing recovery (reload from TODO_LEDGER.md + artifacts)
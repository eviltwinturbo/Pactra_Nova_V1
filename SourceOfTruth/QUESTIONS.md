# QUESTIONS.md

**DEV NOTE:**
- Use this document to surface design, implementation, and governance questions BEFORE execution.
- Each question entry must be answered and linked to a DECISION_LOG entry before execution begins.

---

## Template for questions (fill per item)
- ID: Q-YYYYMMDD-XXX
- Tranche: XXXX
- Question: [clear, single-line question]
- Context: [why this question matters]
- Proposed answers: [list of options]
- Preferred by: [role]
- Linked Decision: DEC-YYYYMMDD-XXX
- Status: Open / Answered / Deferred

---

## Current open questions

### Q-20251101-001
- **Tranche:** 2541-2555
- **Question:** Should Unity Play Mode tests run in local CI or remote hosted runner?
- **Context:** Local environments show instability; remote CI preserves reproducibility.
- **Proposed answers:** 
  - (A) Local only 
  - (B) Remote CI only 
  - (C) Hybrid with gating
- **Preferred by:** Dev
- **Linked Decision:** DEC-20251101-001
- **Status:** Open

---

## Answered questions (historical)

### Q-20251030-001
- **Tranche:** 2526-2540
- **Question:** Should Tranche 2526-2540 defer Unity Play Mode test execution pending environment stabilization?
- **Context:** Unity 6.2 compatibility issues; Unreal tests fully passing; code compiles cleanly.
- **Answer (Selected):** Yes — defer to CI/remediation window; defer to CI by 2025-12-04.
- **Linked Decision:** DEC-20251030-001
- **Status:** Answered & Authorized
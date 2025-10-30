# TODO_LEDGER

**DEV NOTE:**
- This ledger is the authoritative, append-only task queue for all tranches.
- Dev (owner) reviews and signs off on completed items.

--- 

## Open items (ordered)

- [ ] TRANCHE 2541-2555: Implement Certifications + Renewals widgets (owner: Unreal)
- [ ] TRANCHE 2541-2555: Implement UI Toolkit parity (owner: Unity)
- [ ] TRANCHE 2541-2555: Add Play Mode tests (owner: Unity)
- [ ] TRANCHE 2541-2555: Add Unreal automation tests (owner: Unreal)
- [ ] TRANCHE 2541-2555: Capture 12 screenshots and store at artifacts/screenshots/phase4/tranche-2541-2555 (owner: QA)
- [ ] TRANCHE 2541-2555: Generate SHA256 provenance manifest (owner: QA)
- [ ] TRANCHE 2541-2555: Update CHANGELOG.md and PHASE_4_GUI_FEATURES_VALIDATION_REPORT.md (owner: Dev)

--- 

## Completed items

- [x] TRANCHE 2526-2540: Implement 12 widgets (6 Unreal + 6 Unity) — COMPLETE
- [x] TRANCHE 2526-2540: Run 35/35 tests (16 Unreal + 19 Unity) — PASS
- [x] TRANCHE 2526-2540: Generate SHA256 provenance manifest — LOCKED
- [x] TRANCHE 2526-2540: Update governance artifacts — SYNCHRONIZED

--- 

## PROCESS RULES (Dev)
- Every entry MUST include: tranche, artifact(s), owner, target date.
- Only Dev can mark items `[x]` when all acceptance gates for that item are satisfied.
- Cursor must append evidence to each completed item: tests, screenshots, hashes, logs.

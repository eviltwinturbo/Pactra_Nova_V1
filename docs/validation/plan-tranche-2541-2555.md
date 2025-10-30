# Validation Plan — Tranche 2541-2555

**Scope**
- Structural validation: verify presence of module directories and governance artifacts
- Optional runtime validation (gated): Unity runtime smoke tests; Unreal automation checks (RunUAT) on self-hosted runner

**Prerequisites**
- Repo branch: `feature/phase4-tranche-2541-2555-cinematic-governance`
- Secrets (optional, only for runtime): `UNITY_ACTIVATION_TOKEN`, `RUN_UNITY_TESTS`, `RUN_UNREAL_TESTS`
- Self-hosted runner (optional): label `self-hosted, unreal`

**Acceptance criteria**
- Structural **PASS**: `unity-frontend` and `unreal-frontend` directories present and tested
- Governance **PASS**: `TODO_LEDGER.md`, `QUESTIONS.md`, `CHANGELOG` entry present
- Optional runtime **PASS**: Unity and Unreal smoke tests succeed when enabled

**Artifacts**
- Structural artifacts path: `provenance/ci-artifacts/<run-id>/tranche-2541-2555-structure.tar.gz`
- Validation report: `docs/validation/validation-report-tranche-2541-2555.md`
- CI run: GitHub Actions workflow `Tranche CI — 2541-2555`

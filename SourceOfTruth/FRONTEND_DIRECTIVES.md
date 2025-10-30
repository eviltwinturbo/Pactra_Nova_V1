# Pactra Nova — Frontend Directives for Cursor (Coding Agent)

## Scope of Work

Cursor, you are the designated Coding Agent. You must follow these directives exactly:

### 1. Backend is Untouchable

- Do not modify, delete, or restructure any backend files, services, or infrastructure.
- Your scope is frontend only.
- The `pactranova/` directory is completely off-limits.

### 2. Framework Lock

- You are **forbidden** from using web frameworks (React, Next.js, Tailwind, etc.).
- You must build **only** in:
  - **Unreal Engine 5.6** using **Slate UI**.
  - **Unity 2021+** using **Nova UI** (preferred) or **UI Toolkit** if Nova is unavailable.

### 3. Day 0 Baseline

- **Only two UIs are required:**
  - **Orbit Messenger**
  - **Case Agent**
- No other routes, dashboards, or marketing pages may be created unless explicitly approved in `QUESTIONS.md` and surfaced in this document.

### 4. Always Read First

- Before creating or editing any file, you must read this document and `SourceOfTruth/README.md`.
- If instructions conflict, the README is the higher authority.

### 5. Proposal Protocol

- If you believe a new UI artifact is needed, append a proposal to `QUESTIONS.md` with status **Proposed**.
- Do not create the file until governance approval is added here.

---

## Unreal Engine (Slate UI)

### Project Scaffold

- Create a UE 5.6 C++ desktop app with a dedicated Slate module.
- Module layout:
  ```
  Source/PactraNova/
    PactraNova.Build.cs
    UI/Slate/Widgets/
    UI/Styles/
    UI/Windows/
    Services/
    ViewModels/
    Tests/
  ```

### Core Widgets

- `SOrbitSidebar`, `SConversationList`, `SMessagePane`, `SSettingsPane`, `SUpgradeModal`, `STrialBanner`.

### Windows

- `SOrbitWindow` mapped to `/orbit/index`, `/orbit/channels`, `/orbit/settings`.
- `SCaseAgentWindow` for Case Agent functionality.

### Services

- API client for login, tier config, conversations, messages, presence.
- GateOrUpgrade(featureId) → show `SUpgradeModal` if denied.

### Testing

- Unit: widget construction, style application.
- Functional: login, tier gating, message send/receive.
- Visual: screenshot comparisons.

---

## Unity (Nova UI / UI Toolkit)

### Project Scaffold

- Unity 2021+ project with Burst/Jobs enabled if using Nova.
- Folder layout:
  ```
  Assets/PactraNova/UI/
    Styles/
    Prefabs/
    Panels/
    Scripts/Services/
    Scripts/ViewModels/
    Scripts/Controllers/
    Tests/
  ```

### Core Prefabs

- `OrbitSidebar`, `ConversationList`, `MessagePane`, `SettingsPane`, `UpgradeModal`, `TrialBanner`.
- `OrbitPanel`, `CaseAgentPanel`.

### Controllers

- `ApiClient` for login, tier config, conversations, messages, presence.
- GateOrUpgrade(featureId) → show `UpgradeModal`.

### Testing

- Play mode: login, message flow, gating.
- Visual regression: screenshot tests.
- Performance: frame time, allocations (zero per‑frame GC if Nova).

---

## Governance Enforcement

- CI/CD must fail if:
  - Any frontend file exists outside Unreal/Unity projects.
  - `QUESTIONS.md` contains "Proposed" entries without approval here.

- All changes must be surfaced in:
  - `CHANGELOG.md` with SHA256 hashes of updated assets.
  - This document (`FRONTEND_DIRECTIVES.md`) if governance rules change.

---

## Summary for Cursor

- **Do:** Build Orbit Messenger + Case Agent in Unreal Slate and Unity Nova/UI Toolkit.
- **Don't:** Touch backend, create web UIs, or add extra routes without approval.
- **Always:** Read this file + README first, log proposals in `QUESTIONS.md`, surface changes in CHANGELOG.

# Pactra Nova — Native Frontends

This directory contains the authorized native frontend implementations for Pactra Nova using **Unreal Engine 5.6** (Slate UI) and **Unity 2021 LTS** (Nova UI).

## Directory Structure

```
native-frontends/
├── unreal-frontend/           # Unreal Engine 5.6 Slate UI project
│   ├── PactraNova.uproject
│   └── Source/PactraNova/
│       ├── PactraNova.Build.cs
│       ├── UI/Slate/
│       │   ├── Widgets/       # 6 core widgets
│       │   ├── Windows/       # 2 main windows
│       │   └── Styles/        # Slate style definitions
│       ├── Services/          # API client
│       ├── ViewModels/        # Future: business logic
│       └── Tests/             # Unit and functional tests
│
└── unity-frontend/            # Unity 2021 LTS Nova UI project
    ├── ProjectSettings/
    ├── Assets/PactraNova/UI/
    │   ├── Scripts/
    │   │   ├── Controllers/   # 2 main controllers
    │   │   ├── Services/      # API client
    │   │   └── Tests/         # Play mode tests
    │   ├── Prefabs/           # UI panel prefabs
    │   └── Styles/            # Design tokens
    └── ProjectVersion.txt     # 2021.3.0f1
```

## Scope

**Authorized UIs:**
- Orbit Messenger
- Case Agent

**Forbidden:**
- Marketing pages
- Pricing UI
- Dashboards
- Any UI outside this list without governance approval

**Framework Lock:**
- Unreal: Slate UI only (no other frameworks)
- Unity: Nova UI preferred, UI Toolkit fallback

## Getting Started

### Prerequisites

- **Unreal Engine 5.6** (or higher)
  - Download from Epic Games Launcher
  - Visual Studio 2022 (for compilation)
  - ~50GB disk space

- **Unity 2021 LTS** (2021.3.0f1 or compatible)
  - Download from Unity Hub
  - ~15GB disk space

### Local Setup

```bash
# Clone repo and bootstrap environment
git clone <repo-url>
cd <repo-root>

# Run bootstrap script (Linux/Mac/WSL)
./scripts/bootstrap.sh

# Or manually:
chmod +x scripts/*.sh
./scripts/hash-assets.sh
./scripts/enforce-frontend-scope.sh
./scripts/enforce-questions.sh
```

### Building Unreal

```bash
# Open in Unreal Editor
1. Epic Games Launcher → Engine → Launch (UE 5.6)
2. File → Open Project → native-frontends/unreal-frontend/PactraNova.uproject
3. Build → Compile
4. Play to test Orbit Window
```

### Building Unity

```bash
# Open in Unity Hub
1. Unity Hub → Open → native-frontends/unity-frontend
2. Select Unity 2021.3.0f1
3. Play to test OrbitController and CaseAgentController
```

## Architecture

### Unreal Slate Widgets

**Windows:**
- `SOrbitWindow` — Main Orbit Messenger interface
- `SCaseAgentWindow` — Case Agent interface

**Widgets:**
- `SOrbitSidebar` — Navigation and channels
- `SConversationList` — List of conversations
- `SMessagePane` — Message display and input
- `SSettingsPane` — User settings
- `SUpgradeModal` — Tier upgrade dialog
- `STrialBanner` — Trial period indicator

**Services:**
- `FApiClient` — HTTP communication with backend
  - Login, tier config, conversations, messages, presence

### Unity Controllers & Services

**Controllers:**
- `OrbitController` — Orchestrates Orbit UI logic
- `CaseAgentController` — Orchestrates Case Agent UI logic

**Services:**
- `ApiClient` — Async HTTP communication with backend
  - Mirrors Unreal FApiClient interface

## API Integration

Both frontends communicate with the backend via HTTP:

**Endpoints:**
```
POST /v1/auth/login
GET /v1/tiers
GET /v1/orbit/conversations
POST /v1/orbit/messages
PATCH /v1/presence
```

**Configuration:**
```
Base URL: http://localhost:5000 (development)
```

## Governance

### Rules

1. **Backend is Untouchable** — Never modify `pactranova/` directory
2. **Framework Lock** — No web frameworks (React, Next.js, etc.)
3. **Day 0 Baseline** — Only Orbit + Case Agent
4. **Scope Enforcement** — CI/CD validates on every commit
5. **Proposal Protocol** — New UIs require governance approval

### CI/CD Validation

```bash
./scripts/enforce-frontend-scope.sh
# ✅ Verifies only native-frontends/ has frontend code
# ✅ Rejects unauthorized web frameworks
# ✅ Confirms backend untouched

./scripts/enforce-questions.sh
# ✅ Verifies no unresolved proposals

./scripts/hash-assets.sh
# ✅ Generates SHA256 provenance hashes
```

## Expanding the Frontend

**To propose a new UI:**

1. Add to `SourceOfTruth/QUESTIONS.md`:
   ```
   Date: 2025-10-28
   Proposed Artifact: /dashboard
   Reason: KPI monitoring for admins
   Impact: Internal tool value
   Status: Proposed
   ```

2. Governance review → Decision makers approve/reject

3. If approved:
   - Update `SourceOfTruth/FRONTEND_DIRECTIVES.md`
   - Update `SourceOfTruth/QUESTIONS.md` status to "Approved"
   - Implement in Unreal/Unity

4. CI/CD will now allow the new files

## Debugging

### Unreal Debugging

- **Output Log:** Window → Developer Tools → Output Log
- **Visual Studio Debugger:** Attach via Debug → Windows Debugger
- **Insights:** Unreal Insights (File → Session Frontend)

### Unity Debugging

- **Console:** Window → General → Console
- **Visual Studio Debugger:** Tools → Attach to Unity Debugger
- **Profiler:** Window → Analysis → Profiler

## Testing

### Unreal Tests

```
# Compile and run tests
Right-click PactraNova.uproject → Generate Visual Studio project files
Open PactraNova.sln → Build → Run Tests
```

### Unity Tests

```
# Run Play Mode tests
Window → General → Test Runner
Click Run All
```

## Performance Targets

- **Unreal:** 60 FPS on Windows 10+ with RTX 2060 or equivalent
- **Unity:** 60 FPS on Windows 10+ with integrated graphics

## Documentation References

- **Governance:** `SourceOfTruth/README.md`
- **Directives:** `SourceOfTruth/FRONTEND_DIRECTIVES.md`
- **Skeleton Specs:** `SourceOfTruth/FRONTEND_SKELETON.md`
- **Questions:** `SourceOfTruth/QUESTIONS.md`
- **Changelog:** `SourceOfTruth/CHANGELOG.md`

## Support

For questions or issues:

1. Read `SourceOfTruth/FRONTEND_DIRECTIVES.md`
2. Check `SourceOfTruth/QUESTIONS.md` for precedent decisions
3. Log new proposals with governance context
4. Run CI/CD checks locally before pushing

---

**Last Updated:** 2025-10-28  
**Day 0 Reset Status:** ✅ COMPLETE  
**Governance Version:** 2.0

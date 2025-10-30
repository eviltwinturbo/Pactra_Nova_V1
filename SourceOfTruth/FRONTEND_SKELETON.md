# Pactra Nova — Frontend Skeleton Specification

## Directives for Cursor (Coding Agent)

Cursor, you must follow these instructions exactly. Backend is untouchable. Web frameworks are forbidden. Only Unreal Slate UI and Unity UI (Nova/UI Toolkit) are allowed. Only **Orbit Messenger** and **Case Agent** UIs are in scope.

---

## Unreal Engine 5.6 — Slate UI Skeleton

### Project Structure

```
Source/PactraNova/
  PactraNova.Build.cs
  UI/Slate/Widgets/
  UI/Slate/Windows/
  UI/Styles/
  Services/
  ViewModels/
  Tests/
```

### Class Stubs

#### Windows (2 files)

**`SOrbitWindow`** - Main Orbit Messenger window
- Contains `SOrbitSidebar`, `SConversationList`, `SMessagePane`, `STrialBanner`
- Layout: Sidebar (left) + Conversations (center) + Messages (right)

**`SCaseAgentWindow`** - Case Agent window
- Day 0 stub for future Case Agent implementation

#### Widgets (6 files)

**`SOrbitSidebar`**
- Navigation and channel listing
- Direct message list
- Settings/profile access

**`SConversationList`**
- List of channels and DMs
- Selection/highlighting
- Unread indicators (future)

**`SMessagePane`**
- Message display area
- Message input box
- Send button
- Scroll area

**`SSettingsPane`**
- User preferences
- Notification settings
- Appearance settings (future)

**`SUpgradeModal`**
- Tier upgrade prompt
- Feature gating display
- Upgrade button, cancel button

**`STrialBanner`**
- Trial period indicator
- Upgrade CTA
- Dismissible (optional)

#### Services (1 file)

**`FApiClient`**
- HTTP client for backend communication
- Methods:
  - `Login(Username, Password)` → POST `/v1/auth/login`
  - `FetchTierConfig()` → GET `/v1/tiers`
  - `FetchConversations()` → GET `/v1/orbit/conversations`
  - `SendMessage(ConversationId, Text)` → POST `/v1/orbit/messages`
  - `UpdatePresence(Status)` → PATCH `/v1/presence`

#### Styles (1 file)

**`FPactraNovaStyle`**
- Slate style definitions
- Colors, fonts, spacing, padding
- Component styles (buttons, panels, text)

### Testing

- **Unit Tests:** Widget construction, style loading, API stub responses
- **Functional Tests:** Login flow, conversation list population, message send
- **Visual Tests:** Screenshot comparisons

---

## Unity 2021+ — Nova UI / UI Toolkit

### Project Structure

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

### File Stubs

#### Controllers (2 files)

**`OrbitController`** - Orbit Messenger logic
- Manages UI panel initialization
- Handles message sending
- Conversation selection
- Tier gating with GateOrUpgrade()

**`CaseAgentController`** - Case Agent logic
- Day 0 stub
- LoadCase(caseId), SubmitAnalysis(analysis)

#### Services (1 file)

**`ApiClient`** - HTTP client
- Methods:
  - `Login(username, password)` → Task<bool>
  - `FetchTierConfig()` → Task
  - `FetchConversations()` → Task<List<Conversation>>
  - `SendMessage(conversationId, text)` → Task
  - `UpdatePresence(status)` → Task

#### Prefabs (8 files)

**UI Panels:**
- `OrbitSidebar.prefab`
- `ConversationList.prefab`
- `MessagePane.prefab`
- `SettingsPane.prefab`
- `UpgradeModal.prefab`
- `TrialBanner.prefab`

**Main Panels:**
- `OrbitPanel.prefab`
- `CaseAgentPanel.prefab`

#### Styles (1 file)

**`DesignTokens.asset`** - ScriptableObject
- Color tokens (primary, secondary, success, warning, error)
- Typography (fonts, sizes, weights)
- Spacing (padding, margins, gaps)
- Border radius

### Testing

- **Play Mode Tests:** Login, message flow, tier gating
- **Visual Regression:** Screenshot tests
- **Performance:** Frame time, GC allocations

---

## Governance Enforcement

### CI/CD Pipeline

**enforce-scope.yml:**
- Fails if web frontend directories exist outside `archive/`
- Checks for unauthorized `src/app/`, `src/pages/` patterns

**build-unreal.yml:**
- Compiles Unreal with `RunUAT.sh BuildCookRun`
- Platform: Win64, Config: Development

**build-unity.yml:**
- Compiles Unity with `-buildTarget StandaloneWindows64`
- Batch mode, headless

**enforce-questions.yml:**
- Fails if `QUESTIONS.md` has unresolved "Status: Proposed" entries

**provenance.yml:**
- Runs `scripts/hash-assets.sh`
- Verifies SHA256 hashes

---

## Summary for Cursor

- **Do:** Scaffold both engines with stubs provided above.
- **Don't:** Touch backend, create web UIs, or add extra routes.
- **Always:** Read FRONTEND_DIRECTIVES.md and README before acting.

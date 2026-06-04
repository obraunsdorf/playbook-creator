# Rust Migration Plan: Move Business Logic from Dialogs to Controller

The business logic for play lifecycle, navigation, and player mutation is currently split across
`MainDialog` (filtered play list, navigation state) and `PBCPlayView` (current play data, player
edits). The Rust controller already has the right shape (`current_play`, `load_play()`, etc.) but
it is not yet exposed through the bridge. The plan migrates in 5 phases, each independently
verifiable, leaving Qt rendering/drawing in C++.

**Scope boundary:** Qt rendering/drawing (Bezier math, pixel coords, `src/gui/`) and file I/O
(`PBCStorage`) stay in C++ for now.

**Test command:** `ASAN_OPTIONS=detect_leaks=0 ./bin/tests -- --test-base-dir "test"`

---

## Phase 1 — Expose current-play management in the bridge

*Foundation for all other phases. No C++ changes.*

The Rust controller already has `current_play`, `load_play()`, `save_current_play()`,
`create_and_load_new_play()` — none are exposed in `libpbc2/src/bridge/mod.rs`.

Add to `libpbc2/src/bridge/mod.rs` extern "Rust" block:
- `pbc_load_play(name: &str) -> Result<()>` — sets current play in Rust controller
- `pbc_save_current_play() -> Result<()>` — overwrites play in playbook
- `pbc_save_current_play_as(name: String, code: String) -> Result<()>` — rename + save
- `pbc_create_new_play_and_load(name, code, formation) -> Result<()>` — create + set as current
- `pbc_clear_current_play()`
- `pbc_has_current_play() -> bool`
- `pbc_get_current_play() -> Result<Box<Play>>`
- `pbc_set_current_play_comment(comment: String) -> Result<()>`

Implement in `libpbc2/src/bridge/play.rs` — all delegate to existing Rust controller methods.

**Verify:** `cmake --build build && ASAN_OPTIONS=detect_leaks=0 ./bin/tests -- --test-base-dir "test"`

---

## Phase 2 — Remove `PBCPlayView::_currentPlay`

*Depends on Phase 1.*

`PBCPlayView::_currentPlay` is a full C++ clone of the play being rendered — the largest business
state leak. Replace each operation with a bridge call:

- `showPlay(name)` → `pbc2rust::pbc_load_play(name)`
- `createNewPlay()` → `pbc2rust::pbc_create_new_play_and_load()`
- `savePlay()` / `renameAndSavePlay()` → `pbc2rust::pbc_save_current_play[_as]()`
- `saveFormation()` → new bridge fn `pbc_save_current_play_formation_as(name)`
- `repaint()` — fetch player data for rendering via `pbc_get_current_play()` + player accessors
  instead of `_currentPlay->formation()->players()`

Remove `_currentPlay` field from `pbcPlayView.h` once all call sites are migrated.

**Verify:** build + tests + manually open/display/save a play.

---

## Phase 3 — Player mutation through bridge

*Depends on Phase 2.*

`setActivePlayerColor/Route/Name/Nr()` currently mutate C++ objects directly. Add bridge functions:
- `pbc_set_active_player(nr: u32)` — tells Rust which player is selected
- `pbc_set_current_player_color(r: u8, g: u8, b: u8) -> Result<()>`
- `pbc_set_current_player_route(route_name: String) -> Result<()>`
- `pbc_set_current_player_name(name: String) -> Result<()>`
- `pbc_set_current_player_nr(nr: u32) -> Result<()>`

Implement in `libpbc2/src/bridge/play.rs` or a new `libpbc2/src/bridge/player.rs`.

Route *drawing* (Bezier math, pixel coordinates) stays in C++. Only the final assignment —
"player X gets route Y" — goes through the bridge (from `mouseDoubleClickEvent()`).

**Verify:** build + tests + manually change player color/route/name; verify updates persist.

---

## Phase 4 — Play navigation in Rust

*Depends on Phase 1. Parallel-safe with Phases 2 & 3.*

`MainDialog::_currentlySelectedPlays` + `_currentPlay` iterator implement filtered navigation —
pure business state. Move to Rust:

Add to `PBCController` in `libpbc2/src/controller.rs`:
- `filtered_plays: Vec<PlayName>`
- `filter_index: usize`

Add bridge functions:
- `pbc_set_play_filter(names: Vec<String>)` — stores filtered subset in Rust controller
- `pbc_next_play() -> Result<String>` — advances cursor, returns play name
- `pbc_previous_play() -> Result<String>` — goes back, returns play name
- `pbc_get_current_navigation_play_name() -> Result<String>`

Replace in `MainDialog`:
- `openPlay()` → call `pbc2rust::pbc_set_play_filter(names)` + `pbc2rust::pbc_load_play(name)`
- `nextPlay()` / `previousPlay()` → call bridge, then `_playView->showPlay(returned_name)`

Remove `_currentlySelectedPlays` and `_currentPlay` iterator from `mainDialog.h`.

**Verify:** build + tests + open play filter dialog, navigate next/prev; verify correct plays shown.

---

## Phase 5 — Playbook-level operations, then delete C++ models

*Depends on Phases 1–4.*

Expose already-implemented Rust controller functions:
- `pbc_delete_play(name: String) -> Result<()>`
- `pbc_delete_formation(name: String) -> Result<()>`
- `pbc_delete_route(name: String) -> Result<()>`
- `pbc_delete_category(name: String) -> Result<()>`
- `pbc_add_play_to_category(play_name: String, category_name: String) -> Result<()>`

Replace inline calls in `MainDialog::deletePlays()` etc. to use bridge.

Once no C++ code calls `PBCController::getInstance()->getPlaybook()`, delete:
- `src/pbcController.h` / `src/pbcController.cpp`
- `src/models/pbcPlay.h`, `pbcPlaybook.h`, `pbcFormation.h`, `pbcColor.h`, etc.
- Corresponding entries from `src/CMakeLists.txt`

**Verify:** `grep -r "PBCController::getInstance" src/` returns no results.

---

## Relevant Files

| File | Change |
|---|---|
| `libpbc2/src/bridge/mod.rs` | Add all new `extern "Rust"` declarations |
| `libpbc2/src/bridge/play.rs` | Implement new play/player bridge functions |
| `libpbc2/src/controller.rs` | Add `filtered_plays`/`filter_index` fields (Phase 4) |
| `src/gui/pbcPlayView.cpp` / `.h` | Remove `_currentPlay`, replace with bridge calls |
| `src/dialogs/mainDialog.cpp` / `.h` | Remove `_currentlySelectedPlays`, `_currentPlay` |
| `src/pbcController.h` / `.cpp` | Delete in Phase 5 |

## Explicitly Out of Scope

- `PBCStorage` (Boost.Serialization + Botan file I/O) — deferred
- Route Bezier drawing / coordinate conversion — stays in C++ Qt layer
- All of `src/gui/` (rendering) and `src/dialogs/` (Qt UI input) — not being rewritten

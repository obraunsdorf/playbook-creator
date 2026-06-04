# Playbook Creator — Agent Instructions

Free, open-source desktop editor for American (flag) football playbooks. Cross-platform (Linux/macOS/Windows), GPL-licensed. Written in C++14 (Qt5 GUI) with Rust business logic connected via `cxx`.

## Build & Test

**Prerequisites:** Qt5, Boost 1.78+, Botan 1.10+, Rust toolchain, CMake 3.22+.

```bash
# Configure (already done if build/ exists with CMakeCache.txt)
cmake -S . -B build

# Build everything
cmake --build build

# Run tests
ASAN_OPTIONS=detect_leaks=0 ./bin/tests -- --test-base-dir "test"

# Generate documentation
cmake --build build --target documentation
```

Binaries land in `bin/` at the repo root (`bin/PlaybookCreator`, `bin/tests`).

Address Sanitizer is enabled on non-Windows builds (`-fsanitize=address`). Expect ASAN output when tests fail.

## Architecture

Three-layer design:

```
Qt C++ GUI (src/)
    ↓  cxx FFI bridge
Rust core (libpbc2/)
    +
Rust updater (updater/)
```

### Rust layer — `libpbc2/`
- Business logic: plays, formations, routes, categories, players, the playbook.
- Entry point: `libpbc2/src/lib.rs` + `libpbc2/src/controller.rs` (singleton facade).
- FFI boundary declared in `libpbc2/src/bridge/mod.rs` using the `cxx::bridge` macro.
- `build.rs` generates C++ headers; CMake (via [Corrosion](https://github.com/corrosion-rs/corrosion)) copies them to `build/cxxbridge/`.

### C++ layer — `src/`
| Subdirectory | Purpose |
|---|---|
| `src/models/` | Qt-friendly wrappers around Rust domain objects (`pbcPlay`, `pbcPlaybook`, `pbcFormation`, …) |
| `src/gui/` | Drawing/visualization (`pbcGridIronView`, `pbcPlayerView`, `pbcPlayView`, …) |
| `src/dialogs/` | Modal windows for user input (new play, open play, export PDF, …) |
| `src/util/` | Shared infrastructure: `pbcSingleton.h`, `pbcStorage`, `pbcPositionTranslator`, exceptions |
| `src/pbcController.h` | C++ singleton wrapper delegating to the Rust controller via bridge |

Key singleton utilities: `src/util/pbcSingleton.h` (template) and `src/util/pbcStorage.cpp` (Boost.Serialization + Botan encryption for `.pbc` files).

### Updater — `updater/`
Rust static lib providing self-update logic (`self_update` crate). `cbindgen` generates `updater/pbc_updater_bindings.h` so `src/main.cpp` can call it directly.

## Conventions

- **Class/file prefix:** `pbc` lowercase (e.g., `pbcPlay`, `pbcGridIronView`). File names mirror class names.
- **Method naming:** camelCase.
- **Qt meta-object system:** Any class needing signals/slots must inherit `QObject` and be listed in `qt5_wrap_cpp()` in `src/CMakeLists.txt`.
- **New UI files:** Add `.ui` files to `qt5_wrap_ui()` in `src/CMakeLists.txt`; the generated header `ui_<name>.h` lands in `build/src/`.
- **Code style:** Google C++ Style via `tools/cpplint.py`. Run manually: `python tools/checkStyle.py` (uses MD5 tracking; outputs to `hashRegister.txt`). Currently disabled in CMake but expected for contributions.
- **Rust errors:** Use `thiserror`. Bridge functions must not panic; translate errors before crossing the FFI boundary.
- **Design patterns in use:** Singleton (`pbcSingleton.h`), Strategy (`pbcAbstractMovement`), Model-View (Qt), Bridge/FFI (cxx).

## Key Files

| File | Why it matters |
|---|---|
| [`CMakeLists.txt`](CMakeLists.txt) | Root build: Corrosion, Qt, version extraction, ASAN |
| [`src/CMakeLists.txt`](src/CMakeLists.txt) | Source targets, MOC/UI wrapping, cxx bridge include dirs |
| [`libpbc2/src/bridge/mod.rs`](libpbc2/src/bridge/mod.rs) | Entire Rust↔C++ FFI surface |
| [`libpbc2/src/controller.rs`](libpbc2/src/controller.rs) | Rust business logic entry point |
| [`src/pbcController.h`](src/pbcController.h) | C++ side of the controller bridge |
| [`src/util/pbcDeclarations.h`](src/util/pbcDeclarations.h) | Global type aliases & macros |
| [`src/util/pbcExceptions.h`](src/util/pbcExceptions.h) | Exception hierarchy |
| [`test/tests.cpp`](test/tests.cpp) | Boost.Test unit tests |
| [`RUST_MIGRATION_PLAN.md`](RUST_MIGRATION_PLAN.md) | Phased plan for moving business logic from C++ dialogs to Rust |

## Rust Migration Strategy

The long-term goal is to **delete all C++ business-logic types** and have the Qt GUI consume only Rust-owned data through the cxx bridge. The current architecture is intentionally structured to support a gradual migration.

### Type Ownership (Current → Target)

| Type | Current C++ | Rust internal | Bridge (FFI) | Target |
|---|---|---|---|---|
| Color | `PBCColor` class (`src/models/pbcColor.h`) | `PBCColor` struct (`libpbc2/src/types/color.rs`) | `pbc2rust::Color` POD | Delete C++ class; C++ uses bridge POD |
| Point | `PBCDPoint` (Boost.Geometry typedef) | `Point2D` struct (`libpbc2/src/types/point.rs`) | `pbc2rust::Point2D` POD | Delete C++ typedef; C++ uses bridge POD |
| Name types | Bare `std::string` | `PlayName`, `FormationName`, … (`libpbc2/src/types/names.rs`) | `String` / `&str` via bridge | No C++ equivalent needed |

### Three-Type Pattern (during migration)

While migrating, each concept has three representations — this is intentional, not duplication:

```
C++ class (e.g. PBCColor)     ← to be deleted once migration is complete
    ↕  manual conversion
Bridge POD (pbc2rust::Color)  ← permanent thin FFI struct; C++ only ever sees this
    ↕  From/Into impl
Rust internal type (PBCColor) ← permanent; owns all logic (methods, traits, constants)
```

**Do not collapse the bridge POD and the Rust internal type.** cxx bridge types cannot have impl blocks, trait implementations, or constants added outside the bridge macro.

### Migration Steps (per subsystem)

1. Implement the full feature in Rust (`libpbc2/src/models/` or `libpbc2/src/controller.rs`).
2. Expose it through the bridge in `libpbc2/src/bridge/mod.rs` using simple POD types or opaque `Box<T>`.
3. Update the C++ caller to use the bridge function/type instead of its own implementation.
4. Delete the C++ class/file once it has no remaining references.
5. Remove the corresponding entry from `src/CMakeLists.txt` if applicable.

### What C++ Will Look Like After Migration

```cpp
#include "rust/cxx.h"
#include "libpbc2/src/bridge.rs.h"  // generated by cxx

pbc2rust::Color color{255, 0, 0};
pbc2rust::Point2D pos{10.0, 20.0};
auto play = pbc2rust::pbc_get_play("PA Post");
```

The Qt GUI layer (`src/gui/`, `src/dialogs/`) is **not** being rewritten — only the model/business-logic layer moves to Rust.

## Common Pitfalls

- **Adding a new Rust type to the bridge:** Declare it in `libpbc2/src/bridge/mod.rs`, rebuild (`cmake --build build`), and include the generated header from `build/cxxbridge/` in the consuming C++ file.
- **Qt MOC:** Forgetting to add a new `QObject`-derived header to `qt5_wrap_cpp()` in `src/CMakeLists.txt` causes linker errors about missing vtables or undefined `qt_metacall`.
- **Boost/Botan not found:** These are not system packages on CI — see [`ci-scripts/build-boost.sh`](ci-scripts/build-boost.sh) and [`ci-scripts/build-botan.sh`](ci-scripts/build-botan.sh) if CMake can't locate them.
- **ASAN false positives:** Qt internals sometimes trigger ASAN; check the stack trace to confirm the origin before investigating.

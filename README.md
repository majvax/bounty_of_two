# bounty_of_two (Rewrite)

This document describes the ongoing **rewrite from scratch** of `bounty_of_two` on the `rewrite` branch: goals, architecture, technology choices, current progress, and next steps.

## Overview

The rewrite replaces an earlier prototype with a cleaner, modular C++23 codebase focused on:

- Deterministic and testable core logic
- Clear separation between client, server, and shared code (`src/client`, `src/server`, `src/common`)
- Modern dependency management via **CPM.cmake** (fmt, spdlog, SFML 3)
- Extensible rendering and tooling (Dear ImGui integration)

## Motivation / Why Rewrite

Short version: the first version (raylib + some C++23) painted us into a corner.

Problems with the old code:
- Raylib’s **C‑style, immediate API** encouraged mixing rendering, input, and game logic everywhere.
- No multiplayer plan: trying to embed **asio** into that loop created brittle threading / blocking hacks.
- Hard to isolate simulation: no clean tick, logic tied to frame time.
- Resource & lifetime chaos (globals / ad‑hoc ownership) made refactors risky.
- Testing was impractical—everything assumed a window.

Why SFML 3 instead:
- **Pure C++ API** (RAII, namespaces, clearer types) fits our architecture style.
- Modular: graphics, window, (and network module if/when we enable it) can coexist cleanly with game logic.
- Optional event polling + cleaner state model works better with an asynchronous networking layer.
- Easier to carve out a **headless server** (no graphics dependency) while sharing common code.

Rewrite goals in one line: a maintainable, deterministic base that can actually support real multiplayer without duct tape.

> Legacy: the original raylib-based codebase is preserved on the `legacy` branch for reference and historical comparison. It will remain read-only except for critical archival fixes (e.g., build script clarity).

### Build System Choice (Back to CMake after premake5)

We briefly experimented with **premake5** but reverted to **CMake** for the rewrite because:
- Dependency management: no first-class, batteries‑included mechanism (vs CPM.cmake / FetchContent / find_package).
- Ecosystem maturity: far more examples, docs, blog posts, and community troubleshooting for CMake.
- Tooling & IDEs: native integration with CLion, VS, VS Code CMake Tools, Ninja multi-config, presets, export compile_commands.
- Cross‑platform CI: standardized workflows on GitHub Actions.
- Feature depth: generator expressions, transitive usage requirements (`target_link_libraries` PUBLIC/PRIVATE/INTERFACE), config-package generation.
- Debuggability: `message()` tracing, `--trace-expand`, built-in file/variable introspection.

Premake5 was pleasant for small single-target prototypes, but scaling to a multi-module project with external libraries and future install/export goals made CMake the more sustainable choice.

## Tech Stack

| Area | Choice | Notes |
|------|--------|-------|
| Language | C++23 | Using modules-ready toolchains (future transition possible) |
| Build | CMake (>=3.30) | Out-of-source builds; CPM for dependencies |
| Windowing / Graphics | SFML 3.0.x | Modern event API (optional events, states) |
| Logging | spdlog + fmt | Structured, fast logging |
| UI / Tooling | Dear ImGui | Via imgui-sfml |

## Directory Layout

```
src/
	common/        # Shared abstractions (to be populated)
	client/        # Rendering loop, input, presentation code
	server/        # Simulation / authoritative logic (skeleton)
cmake/           # CPM & helper CMake modules
Dependencies.cmake  # All third-party pulls via CPM
```

### Current Client Prototype

Implements:
1. Fullscreen SFML 3 window (desktop resolution)
2. Rotating wireframe cube (software 3D: manual projection + perspective)
3. Thick line rendering via triangle expansion (custom CPU geometry)
4. VSync + framerate capping
5. Dynamic centered title text with font auto-discovery (Windows system fonts fallback)

Planned additions:
* Dear ImGui overlay (performance graphs, tweakables)
* Camera abstraction + multiple projection modes
* ECS or lightweight component model for scene entities
* Frame pacing metrics (moving average, jitter graph)

## Dependency Strategy

All third-party libraries are declared in `Dependencies.cmake` using **CPM.cmake**. Advantages:

- Lock to specific tags/versions
- Automatic caching of `_deps` folder
- Easy upgrades (single-line tag change)


## Logging

`spdlog` logs startup video mode and shutdown events. Future logging tasks:
- Structured event timeline (JSON optional)
- Compile-time log level control

## Build Instructions

```powershell
# Configure
cmake -S . -B build

# Build (Release example)
cmake --build build --config Release -j 6

# Run client
./bin/client.exe
```

## Roadmap (Incremental)

| Phase | Focus | Status |
|-------|-------|--------|
| 1 | Baseline window + render loop | ✅ Done |
| 2 | ImGui bootstrap | ✅ Done |
| 3 | Core simulation module (server/) | ⏳ Pending |
| 4 | Networking layer scaffold | ⏳ Pending |





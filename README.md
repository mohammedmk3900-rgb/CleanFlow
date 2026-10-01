# CleanFlow

**Photoshop Switching Workspace**

CleanFlow is a Windows-first desktop utility for manga/manhwa cleaners. It reduces waiting and context-switching overhead while Photoshop remains the cleaner's primary and fully manual tool.

## What CleanFlow does

- Discovers visible Photoshop windows.
- Maintains four stable switching slots.
- Provides F1-F4 global switching when the keys are available.
- Restores minimized Photoshop windows.
- Verifies foreground activation after switching.
- Reports unavailable slots and hotkey conflicts.
- Keeps the switching layer independent from Photoshop document editing.

## What CleanFlow does not do

CleanFlow does **not**:

- clean images;
- use AI to clean pages;
- run Photoshop Actions;
- batch-process documents;
- modify documents or layers;
- automate the cleaner's decisions;
- inject into Photoshop;
- bypass licensing, activation, mutexes, or security controls.

## Architecture

- C++23
- Qt 6 / QML
- CMake + Ninja
- Win32 platform integration
- Small domain registry with no Windows dependency

The application intentionally uses a modular desktop architecture rather than distributed services or unnecessary infrastructure.

## Validation boundary

Independent concurrent Photoshop instances are a runtime hypothesis, not an assumption.

The client's actual Photoshop installation must validate:

1. independent instance creation;
2. independent windows;
3. concurrent processing;
4. switching while another instance is processing;
5. save/close behavior;
6. resource impact.

CleanFlow will not bypass Photoshop restrictions to force this behavior.

## Build

Use a Qt 6 C++23 environment with CMake and Ninja.

Example:

```text
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

Windows is the primary runtime target.

## Status

Switching foundation / MVP implementation. Runtime validation on the target Windows + Photoshop environment remains required.

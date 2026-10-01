# CleanFlow Architecture

## Product boundary

CleanFlow is a Windows-first Photoshop switching layer. Photoshop remains responsible for cleaning, editing, Actions, documents, layers, saving, and quality decisions.

## Requirements

### Functional

- Discover visible top-level Photoshop windows.
- Maintain four stable user-facing slots.
- Assign newly discovered windows to free slots.
- Preserve surviving slot assignments across refreshes.
- Restore minimized Photoshop windows.
- Switch with F1-F4 when the corresponding global hotkey is registered.
- Provide explicit UI status for slot availability and hotkey registration.
- Verify foreground activation after a switch attempt.

### Non-functional

- No Photoshop document manipulation.
- No privilege escalation requirement.
- No licensing, activation, mutex, or security bypass.
- Native Windows behavior must be treated as fallible and reported as such.
- Core slot management must remain independently testable.

## Architecture

```
QML
 │
 ▼
PhotoshopWindowManager
 │
 ├── PhotoshopSlotRegistry
 │
 └── Windows / Win32
      ├── EnumWindows
      ├── Process inspection
      ├── RegisterHotKey
      ├── WM_HOTKEY
      └── SetForegroundWindow
```

The domain registry has no Win32 dependency. Windows-specific behavior remains in the platform adapter.

## Slot identity

The public slot number is not based on current `EnumWindows` ordering.

A discovered window is assigned to the first free slot. A known window keeps its existing slot during refresh. If a tracked window disappears, its slot becomes free.

This provides stable behavior without introducing persistent storage before it is needed.

## Native event handling

Windows global hotkeys are registered through `RegisterHotKey`. Qt receives system-wide `WM_HOTKEY` messages through `QAbstractNativeEventFilter`.

Hotkey registration is tracked independently for F1, F2, F3, and F4. A conflict with one key does not make the other keys appear healthy.

## Foreground activation

Switching uses:

1. Validate the tracked HWND.
2. Restore it if minimized.
3. Call `SetForegroundWindow`.
4. Verify `GetForegroundWindow()`.
5. Report failure when Windows refuses activation.

No thread-input attachment or forced foreground hacks are used in this stage.

## Multi-instance assumption

Independent concurrent Photoshop instances are **not assumed**.

The actual target Photoshop installation must validate:

- multiple independent processes,
- independent windows,
- concurrent operations,
- independent documents,
- switching behavior,
- save/close behavior,
- resource impact.

CleanFlow will not bypass licensing, activation, mutexes, or security controls to create instances.

## Explicit non-goals

- AI cleaning
- automatic cleaning
- Photoshop Actions
- batch processing
- document/layer manipulation
- file transformation
- process injection
- mutex bypass
- licensing bypass

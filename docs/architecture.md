# CleanFlow Architecture

## Product Principle

CleanFlow is a switching layer around Photoshop.

The cleaner remains in control of Photoshop. CleanFlow reduces dead time and context-switching overhead by making multiple Photoshop workspaces quick to reach.

## Current scope

- Photoshop window discovery
- Workspace slots
- Focus and restore
- Global F1-F4 switching
- Minimal UI

CleanFlow does **not** perform cleaning work.

### Explicitly out of scope

- AI cleaning
- Automatic cleaning
- Photoshop Actions
- Batch processing
- Document manipulation
- Layer manipulation
- File transformation
- Licensing or activation bypasses
- Mutex or security bypasses

## Layering

```text
UI
 │
 ▼
Workspace / Switching Domain
 │
 ▼
Photoshop Platform Adapter
 │
 ▼
Windows / Win32
```

The domain layer describes workspace slots without knowing about Windows or HWNDs. The platform layer discovers Photoshop windows and performs focus/restore operations. The UI exposes the current workspaces and lets the cleaner switch manually.

## Validation boundary

Multiple Photoshop instances are a product dependency to validate on the client's actual Photoshop installation.

CleanFlow must not bypass Photoshop licensing, activation, mutexes, or security controls to create additional instances.

## Design rule

If a feature changes what Photoshop edits, that feature does not belong in the switching layer.

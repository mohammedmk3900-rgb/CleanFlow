# Photoshop Switching MVP

## Goal

CleanFlow reduces cleaner idle time by making it fast to switch between independent Photoshop workspaces.

CleanFlow does not clean pages, make editing decisions, run AI cleaning, or replace Photoshop.

## MVP behavior

- Detect visible Photoshop.exe top-level windows.
- Show detected Photoshop windows in slot order.
- Register F1 through F4 as global switching hotkeys on Windows.
- F1 focuses slot 1, F2 slot 2, F3 slot 3, and F4 slot 4.
- Restore a minimized Photoshop window before focusing it.
- Refresh the detected window list from the UI.
- Never modify the Photoshop document itself.

## Important limitation

This MVP discovers Photoshop processes/windows that already exist. It does not attempt to bypass Photoshop licensing, activation, mutexes, or other application controls.

Whether a particular Photoshop installation can maintain multiple independent instances remains a validation item.

## Acceptance criteria

1. With one visible Photoshop window, slot 1 can be focused with F1.
2. With multiple independently running Photoshop windows, each detected window receives its own slot.
3. F1 through F4 focus the corresponding detected window.
4. A minimized detected window is restored and focused.
5. Closing a Photoshop window and pressing Refresh removes it from the list.
6. No Photoshop document content is modified by the switcher.

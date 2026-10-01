# Cleaner Workflow

CleanFlow is designed around a human-controlled Photoshop workflow.

1. The cleaner opens or prepares a page in Photoshop.
2. The cleaner performs cleaning manually.
3. A Photoshop operation may take time.
4. If another Photoshop slot is available, the cleaner switches to it.
5. The cleaner continues working instead of waiting.
6. The cleaner returns to the previous slot when needed.

## CleanFlow responsibilities

- Discover Photoshop windows.
- Maintain stable slots.
- Provide F1-F4 switching.
- Restore minimized windows.
- Verify foreground activation.
- Report switching failures.

## Photoshop responsibilities

- Cleaning decisions.
- Tools.
- Documents.
- Layers.
- Actions.
- Saving.
- Export.
- Quality control.

CleanFlow does not infer whether Photoshop is currently processing an operation. A future status integration would require evidence from a supported Photoshop interface; it must not guess.

## Operational principle

The objective is not to maximize the number of Photoshop instances.

The objective is to reduce productive time lost to waiting, subject to the client's real hardware and Photoshop behavior.

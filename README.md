# CleanFlow

**Parallel Workspace for Manga & Manhwa Cleaning**

CleanFlow is a Windows-first workflow orchestrator designed to reduce idle time in manga and manhwa cleaning workflows.

The core idea is simple:

> Photoshop remains the cleaner's main tool. CleanFlow removes waiting time around Photoshop instead of replacing it.

## Goals

- Track independent Photoshop workers.
- Queue and assign cleaning jobs.
- Reduce idle time while Photoshop processes an operation.
- Keep input, working, and output files clearly separated.
- Verify outputs before marking jobs complete.
- Support fast human switching between workers.
- Keep the workflow local-first.

## Initial Architecture

- C++23
- Qt / QML
- CMake + Ninja
- SQLite
- Win32 platform integration

## Important Constraint

Multi-instance Photoshop support is **not assumed**.

The first implementation milestone is a proof of concept that verifies whether the target Photoshop version can support genuinely independent concurrent workers and whether actions/processes can run concurrently without bypassing licensing or security mechanisms.

## Non-Goals

CleanFlow is not intended to:

- Replace Photoshop.
- Automatically redraw every page.
- Be an AI manga cleaner.
- Circumvent Photoshop licensing or security.
- Overwrite original source files.

## Status

Early architecture / proof-of-concept stage.

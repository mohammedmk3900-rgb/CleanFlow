# CleanFlow Architecture

## Product Principle

CleanFlow is a workflow orchestrator around Photoshop. It reduces cleaner idle time without replacing the human-controlled Photoshop workflow.

## Components

- UI: Worker Grid, Queue, Project, Status
- Orchestrator: Scheduler, Job Manager, Worker Manager, Session Manager
- Photoshop Integration: Adapter, Action Runner, Document Controller, State Detector
- Human Workflow: Focus Switching, Global Hotkeys
- File Pipeline: Input, Working, Output, Verification
- Persistence: SQLite

## Worker Lifecycle

OFFLINE -> STARTING -> READY -> WORKING/PROCESSING -> VALIDATING -> READY

Errors transition to ERROR and may be retried.

## Job Lifecycle

QUEUED -> ASSIGNED -> STARTING -> PROCESSING -> VALIDATING -> COMPLETED

Failures become FAILED and can transition to RETRY.

## Multi-Instance Proof of Concept

The first platform milestone is deliberately small:

1. Detect or launch Photoshop.
2. Enumerate Photoshop processes and windows.
3. Track PID and window handles.
4. Determine whether multiple independent workers can coexist on the target installation.
5. Determine whether concurrent actions can actually execute.
6. Verify output files.

CleanFlow must not bypass licensing, activation, or security controls.

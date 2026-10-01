#pragma once
namespace cleanflow {
enum class WorkspaceState { Empty, Ready, Active, Busy, Unavailable };
inline const char* workspaceStateName(WorkspaceState s) {
    switch (s) {
    case WorkspaceState::Empty: return "Empty";
    case WorkspaceState::Ready: return "Ready";
    case WorkspaceState::Active: return "Active";
    case WorkspaceState::Busy: return "Busy";
    case WorkspaceState::Unavailable: return "Unavailable";
    }
    return "Unknown";
}
} // namespace cleanflow

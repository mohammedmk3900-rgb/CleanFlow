#include "WorkspaceRegistry.h"
namespace cleanflow {
WorkspaceRegistry::WorkspaceRegistry() {
    m_slots.reserve(kMaxSlots);
    for (int i = 1; i <= kMaxSlots; ++i) {
        WorkspaceSlot slot;
        slot.number = i;
        slot.shortcut = QStringLiteral("F%1").arg(i);
        m_slots.append(std::move(slot));
    }
}
void WorkspaceRegistry::reset() {
    for (auto& slot : m_slots) {
        const int number = slot.number;
        const QString shortcut = slot.shortcut;
        slot = {};
        slot.number = number;
        slot.shortcut = shortcut;
    }
}
bool WorkspaceRegistry::assign(int slotNumber, quintptr handle, quint64 pid, const QString& title) {
    if (slotNumber < 1 || slotNumber > m_slots.size() || handle == 0) return false;
    auto& slot = m_slots[slotNumber - 1];
    slot.windowHandle = handle;
    slot.processId = pid;
    slot.windowTitle = title;
    slot.state = WorkspaceState::Ready;
    return true;
}
bool WorkspaceRegistry::updateState(int slotNumber, WorkspaceState state) {
    if (slotNumber < 1 || slotNumber > m_slots.size()) return false;
    m_slots[slotNumber - 1].state = state;
    return true;
}
const WorkspaceSlot* WorkspaceRegistry::find(int slotNumber) const noexcept {
    if (slotNumber < 1 || slotNumber > m_slots.size()) return nullptr;
    return &m_slots[slotNumber - 1];
}
}
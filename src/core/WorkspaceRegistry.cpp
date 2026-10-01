#include "WorkspaceRegistry.h"

namespace cleanflow {

WorkspaceRegistry::WorkspaceRegistry()
{
    m_slots.reserve(kMaxSlots);

    for (int i = 0; i < kMaxSlots; ++i)
    {
        WorkspaceSlot slot;
        slot.number = i + 1;
        slot.label = QStringLiteral("F%1").arg(i + 1);
        m_slots.append(slot);
    }
}

void WorkspaceRegistry::clear()
{
    for (auto& slot : m_slots)
    {
        slot.windowTitle.clear();
        slot.available = false;
    }
}

void WorkspaceRegistry::setWindow(int slot, const QString& title)
{
    const int index = slot - 1;
    if (index < 0 || index >= m_slots.size())
        return;

    m_slots[index].windowTitle = title;
    m_slots[index].available = true;
}

const QVector<WorkspaceSlot>& WorkspaceRegistry::slots() const
{
    return m_slots;
}

} // namespace cleanflow

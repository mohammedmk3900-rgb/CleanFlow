#pragma once
#include "WorkspaceSlot.h"
#include <QVector>
namespace cleanflow {
class WorkspaceRegistry final {
public:
    static constexpr int kMaxSlots = 4;
    WorkspaceRegistry();
    void reset();
    bool assign(int slot, quintptr windowHandle, quint64 processId, const QString& title);
    bool updateState(int slot, WorkspaceState state);
    const QVector<WorkspaceSlot>& all() const noexcept { return m_slots; }
    const WorkspaceSlot* find(int slot) const noexcept;
private:
    QVector<WorkspaceSlot> m_slots;
};
}
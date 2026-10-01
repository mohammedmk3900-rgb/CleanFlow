#pragma once

#include "WorkspaceSlot.h"

#include <QVector>

namespace cleanflow {

class WorkspaceRegistry final
{
public:
    static constexpr int kMaxSlots = 4;

    WorkspaceRegistry();

    void clear();
    void setWindow(int slot, const QString& title);
    const QVector<WorkspaceSlot>& slots() const;

private:
    QVector<WorkspaceSlot> m_slots;
};

} // namespace cleanflow

#pragma once
#include <QObject>
#include "WorkspaceRegistry.h"
namespace cleanflow {
class SwitchingService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(int activeSlot READ activeSlot NOTIFY activeSlotChanged)
public:
    explicit SwitchingService(QObject* parent=nullptr);
    const WorkspaceRegistry& registry() const { return m_registry; }
    int activeSlot() const { return m_activeSlot; }
    void setActiveSlot(int slot);
signals:
    void activeSlotChanged();
private:
    WorkspaceRegistry m_registry;
    int m_activeSlot=0;
};
} // namespace cleanflow

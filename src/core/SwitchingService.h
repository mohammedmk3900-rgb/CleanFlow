#pragma once
#include <QObject>
namespace cleanflow {
class SwitchingService : public QObject {
    Q_OBJECT
    Q_PROPERTY(int activeSlot READ activeSlot NOTIFY activeSlotChanged)
public:
    explicit SwitchingService(QObject* parent = nullptr) : QObject(parent) {}
    int activeSlot() const noexcept { return m_activeSlot; }
    bool setActiveSlot(int slot) {
        if (slot < 0 || slot > 4 || slot == m_activeSlot) return false;
        m_activeSlot = slot;
        emit activeSlotChanged();
        return true;
    }
signals:
    void activeSlotChanged();
private:
    int m_activeSlot = 0;
};
}
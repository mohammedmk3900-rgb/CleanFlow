#pragma once
#include "PhotoshopSlot.h"
#include <QVector>

namespace cleanflow {

class PhotoshopSlotRegistry final {
public:
    static constexpr int kSlotCount = 4;

    PhotoshopSlotRegistry();

    PhotoshopSlot& at(int number);
    const PhotoshopSlot& at(int number) const;
    const PhotoshopSlot* findByHandle(quintptr handle) const noexcept;
    int slotForHandle(quintptr handle) const noexcept;
    int firstFreeSlot() const noexcept;

    bool assign(int number, quint64 processId, quintptr windowHandle, const QString& title);
    void release(int number);

    const QVector<PhotoshopSlot>& slots() const noexcept { return m_slots; }

private:
    QVector<PhotoshopSlot> m_slots;
};

}
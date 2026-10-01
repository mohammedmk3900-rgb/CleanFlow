#include "PhotoshopSlotRegistry.h"

namespace cleanflow {

PhotoshopSlotRegistry::PhotoshopSlotRegistry()
{
    m_slots.reserve(kSlotCount);
    for (int i = 1; i <= kSlotCount; ++i) {
        PhotoshopSlot slot;
        slot.number = i;
        slot.shortcut = QStringLiteral("F%1").arg(i);
        m_slots.append(slot);
    }
}

PhotoshopSlot& PhotoshopSlotRegistry::at(int number)
{
    Q_ASSERT(number >= 1 && number <= m_slots.size());
    return m_slots[number - 1];
}

const PhotoshopSlot& PhotoshopSlotRegistry::at(int number) const
{
    Q_ASSERT(number >= 1 && number <= m_slots.size());
    return m_slots[number - 1];
}

const PhotoshopSlot* PhotoshopSlotRegistry::findByHandle(quintptr handle) const noexcept
{
    if (!handle)
        return nullptr;

    for (const auto& slot : m_slots) {
        if (slot.windowHandle == handle)
            return &slot;
    }

    return nullptr;
}

int PhotoshopSlotRegistry::slotForHandle(quintptr handle) const noexcept
{
    const auto* slot = findByHandle(handle);
    return slot ? slot->number : 0;
}

int PhotoshopSlotRegistry::firstFreeSlot() const noexcept
{
    for (const auto& slot : m_slots) {
        if (slot.windowHandle == 0)
            return slot.number;
    }

    return 0;
}

bool PhotoshopSlotRegistry::assign(
    int number,
    quint64 processId,
    quintptr windowHandle,
    const QString& title)
{
    if (number < 1 || number > m_slots.size() || windowHandle == 0)
        return false;

    auto& slot = at(number);
    slot.processId = processId;
    slot.windowHandle = windowHandle;
    slot.title = title;
    slot.state = PhotoshopSlotState::Available;
    return true;
}

void PhotoshopSlotRegistry::release(int number)
{
    if (number < 1 || number > m_slots.size())
        return;

    auto& slot = at(number);
    const QString shortcut = slot.shortcut;
    const int slotNumber = slot.number;

    slot = {};
    slot.number = slotNumber;
    slot.shortcut = shortcut;
}

}
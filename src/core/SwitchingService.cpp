#include "SwitchingService.h"
namespace cleanflow {
SwitchingService::SwitchingService(QObject* parent):QObject(parent){}
void SwitchingService::setActiveSlot(int slot) {
    if (slot<0 || slot>WorkspaceRegistry::kMaxSlots || slot==m_activeSlot) return;
    m_activeSlot=slot;
    emit activeSlotChanged();
}
} // namespace cleanflow

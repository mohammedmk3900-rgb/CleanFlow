#pragma once
#include <QString>
#include <QtGlobal>

namespace cleanflow {

enum class PhotoshopSlotState {
    Empty,
    Available,
    Active,
    Unavailable
};

struct PhotoshopSlot {
    int number = 0;
    QString shortcut;
    QString title;
    quint64 processId = 0;
    quintptr windowHandle = 0;
    PhotoshopSlotState state = PhotoshopSlotState::Empty;
};

}
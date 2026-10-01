#pragma once
#include "WorkspaceState.h"
#include <QString>
#include <QtGlobal>
namespace cleanflow {
struct WorkspaceSlot {
    int number = 0;
    QString shortcut;
    QString windowTitle;
    quintptr windowHandle = 0;
    quint64 processId = 0;
    WorkspaceState state = WorkspaceState::Empty;
    bool isAvailable() const noexcept { return windowHandle != 0; }
};
}
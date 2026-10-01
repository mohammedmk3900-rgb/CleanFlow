#pragma once

#include <QString>

namespace cleanflow {

struct WorkspaceSlot
{
    int number = 0;
    QString label;
    QString windowTitle;
    bool available = false;
};

} // namespace cleanflow

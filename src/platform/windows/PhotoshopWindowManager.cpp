#include "PhotoshopWindowManager.h"

#include <QAbstractNativeEventFilter>
#include <QCoreApplication>

#ifdef Q_OS_WIN

namespace {
constexpr int kHotkeyBase = 0xCF10;
constexpr int kFirstSlot = 1;
constexpr int kLastSlot = 4;

class HotkeyFilter final : public QAbstractNativeEventFilter
{
public:
    explicit HotkeyFilter(cleanflow::PhotoshopWindowManager* manager)
        : m_manager(manager)
    {
    }

    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr*) override
    {
        if (eventType != "windows_generic_MSG" && eventType != "windows_dispatcher_MSG")
            return false;

        auto* msg = static_cast<MSG*>(message);
        return m_manager->handleNativeMessage(msg);
    }

private:
    cleanflow::PhotoshopWindowManager* m_manager;
};

} // namespace

#endif

namespace cleanflow {

PhotoshopWindowManager::PhotoshopWindowManager(QObject* parent)
    : QObject(parent)
{
#ifdef Q_OS_WIN
    registerHotkeys();
    QCoreApplication::instance()->installNativeEventFilter(
        new HotkeyFilter(this));
#endif
    refresh();
}

PhotoshopWindowManager::~PhotoshopWindowManager()
{
#ifdef Q_OS_WIN
    unregisterHotkeys();
#endif
}

QStringList PhotoshopWindowManager::windows() const
{
    return m_windows;
}

void PhotoshopWindowManager::refresh()
{
#ifdef Q_OS_WIN
    m_windows.clear();
    m_handles.clear();

    EnumWindows(&PhotoshopWindowManager::enumWindowsProc,
                reinterpret_cast<LPARAM>(this));
#endif
    emit windowsChanged();
}

bool PhotoshopWindowManager::switchTo(int slot)
{
#ifdef Q_OS_WIN
    const int index = slot - 1;
    if (index < 0 || index >= m_handles.size())
        return false;

    const HWND hwnd = m_handles.at(index);
    if (!IsWindow(hwnd))
    {
        refresh();
        return false;
    }

    if (IsIconic(hwnd))
        ShowWindow(hwnd, SW_RESTORE);

    SetForegroundWindow(hwnd);
    return GetForegroundWindow() == hwnd;
#else
    Q_UNUSED(slot);
    return false;
#endif
}

#ifdef Q_OS_WIN

BOOL CALLBACK PhotoshopWindowManager::enumWindowsProc(HWND hwnd, LPARAM lParam)
{
    auto* manager = reinterpret_cast<PhotoshopWindowManager*>(lParam);
    if (!manager->isPhotoshopWindow(hwnd))
        return TRUE;

    manager->m_handles.append(hwnd);
    manager->m_windows.append(manager->windowTitle(hwnd));
    return TRUE;
}

bool PhotoshopWindowManager::isPhotoshopWindow(HWND hwnd) const
{
    if (!IsWindowVisible(hwnd))
        return false;

    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == 0)
        return false;

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!process)
        return false;

    wchar_t path[MAX_PATH] = {};
    DWORD size = MAX_PATH;
    const bool ok = QueryFullProcessImageNameW(process, 0, path, &size);
    CloseHandle(process);

    if (!ok)
        return false;

    const QString exePath = QString::fromWCharArray(path);
    return exePath.endsWith(QStringLiteral("\\Photoshop.exe"), Qt::CaseInsensitive);
}

QString PhotoshopWindowManager::windowTitle(HWND hwnd) const
{
    wchar_t title[512] = {};
    const int length = GetWindowTextW(hwnd, title, 512);
    if (length <= 0)
        return QStringLiteral("Adobe Photoshop");

    return QString::fromWCharArray(title, length);
}

bool PhotoshopWindowManager::registerHotkeys()
{
    bool success = true;
    for (int slot = kFirstSlot; slot <= kLastSlot; ++slot)
    {
        if (!RegisterHotKey(nullptr, kHotkeyBase + slot, 0, VK_F1 + slot - 1))
            success = false;
    }
    return success;
}

void PhotoshopWindowManager::unregisterHotkeys()
{
    for (int slot = kFirstSlot; slot <= kLastSlot; ++slot)
        UnregisterHotKey(nullptr, kHotkeyBase + slot);
}

bool PhotoshopWindowManager::handleNativeMessage(MSG* message)
{
    if (!message || message->message != WM_HOTKEY)
        return false;

    const int slot = static_cast<int>(message->wParam) - kHotkeyBase;
    if (slot < kFirstSlot || slot > kLastSlot)
        return false;

    switchTo(slot);
    return true;
}

#endif

} // namespace cleanflow

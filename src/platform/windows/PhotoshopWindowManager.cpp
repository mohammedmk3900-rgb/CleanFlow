#include "PhotoshopWindowManager.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QTimer>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace cleanflow {

namespace {
#ifdef Q_OS_WIN
constexpr int kHotkeyBase = 0xCF10;
#endif

QString stateName(PhotoshopSlotState state)
{
    switch (state) {
    case PhotoshopSlotState::Empty:
        return QStringLiteral("Empty");
    case PhotoshopSlotState::Available:
        return QStringLiteral("Available");
    case PhotoshopSlotState::Active:
        return QStringLiteral("Active");
    case PhotoshopSlotState::Unavailable:
        return QStringLiteral("Unavailable");
    }

    return QStringLiteral("Unknown");
}
}

PhotoshopWindowManager::PhotoshopWindowManager(QObject* parent)
    : QObject(parent)
{
#ifdef Q_OS_WIN
    QCoreApplication::instance()->installNativeEventFilter(this);

    registerHotkeys();

    auto* timer = new QTimer(this);
    timer->setInterval(300);
    connect(timer, &QTimer::timeout, this, [this] {
        syncActiveSlot();
    });
#endif

    refresh();
}

PhotoshopWindowManager::~PhotoshopWindowManager()
{
#ifdef Q_OS_WIN
    QCoreApplication::instance()->removeNativeEventFilter(this);
    unregisterHotkeys();
#endif
}

QStringList PhotoshopWindowManager::slotTitles() const
{
    QStringList result;
    for (const auto& slot : m_registry.slots())
        result.append(slot.title);
    return result;
}

QStringList PhotoshopWindowManager::slotStates() const
{
    QStringList result;
    for (const auto& slot : m_registry.slots())
        result.append(stateName(slot.state));
    return result;
}

QStringList PhotoshopWindowManager::hotkeyStates() const
{
    QStringList result;

#ifdef Q_OS_WIN
    for (int i = 0; i < PhotoshopSlotRegistry::kSlotCount; ++i) {
        result.append(m_hotkeys[i]
            ? QStringLiteral("Registered")
            : QStringLiteral("Unavailable"));
    }
#else
    for (int i = 0; i < PhotoshopSlotRegistry::kSlotCount; ++i)
        result.append(QStringLiteral("Windows only"));
#endif

    return result;
}

void PhotoshopWindowManager::refresh()
{
#ifdef Q_OS_WIN
    QVector<Candidate> candidates;

    EnumWindows(
        &PhotoshopWindowManager::enumWindowsProc,
        reinterpret_cast<LPARAM>(&candidates));

    bool seen[PhotoshopSlotRegistry::kSlotCount] {};

    for (const auto& candidate : candidates) {
        const quintptr handle =
            reinterpret_cast<quintptr>(candidate.handle);

        const int existing = m_registry.slotForHandle(handle);

        if (existing > 0) {
            m_registry.assign(
                existing,
                candidate.processId,
                handle,
                candidate.title);
            seen[existing - 1] = true;
            continue;
        }

        const int freeSlot = m_registry.firstFreeSlot();
        if (freeSlot == 0)
            continue;

        m_registry.assign(
            freeSlot,
            candidate.processId,
            handle,
            candidate.title);

        seen[freeSlot - 1] = true;
    }

    for (int i = 1; i <= PhotoshopSlotRegistry::kSlotCount; ++i) {
        if (!seen[i - 1] && m_registry.at(i).windowHandle != 0)
            m_registry.release(i);
    }

    syncActiveSlot();
#endif

    emit stateChanged();
}

bool PhotoshopWindowManager::switchTo(int slotNumber)
{
#ifdef Q_OS_WIN
    if (slotNumber < 1 ||
        slotNumber > PhotoshopSlotRegistry::kSlotCount) {
        emit switchFailed(slotNumber, QStringLiteral("Invalid slot"));
        return false;
    }

    const auto& slot = m_registry.at(slotNumber);

    if (!isTrackedWindowValid(slot)) {
        refresh();
        emit switchFailed(
            slotNumber,
            QStringLiteral("Photoshop window is no longer available"));
        return false;
    }

    const HWND hwnd =
        reinterpret_cast<HWND>(slot.windowHandle);

    if (!activateWindow(hwnd)) {
        emit switchFailed(
            slotNumber,
            QStringLiteral("Windows rejected foreground activation"));
        return false;
    }

    syncActiveSlot();
    emit stateChanged();

    return m_activeSlot == slotNumber;
#else
    Q_UNUSED(slotNumber);
    return false;
#endif
}

bool PhotoshopWindowManager::nativeEventFilter(
    const QByteArray& eventType,
    void* message,
    qintptr*)
{
#ifdef Q_OS_WIN
    if (eventType != "windows_dispatcher_MSG" &&
        eventType != "windows_generic_MSG") {
        return false;
    }

    auto* msg = static_cast<MSG*>(message);
    if (!msg || msg->message != WM_HOTKEY)
        return false;

    const int slot =
        static_cast<int>(msg->wParam) - kHotkeyBase;

    if (slot < 1 || slot > PhotoshopSlotRegistry::kSlotCount)
        return false;

    switchTo(slot);
    return true;
#else
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    return false;
#endif
}

#ifdef Q_OS_WIN

BOOL CALLBACK PhotoshopWindowManager::enumWindowsProc(
    HWND hwnd,
    LPARAM parameter)
{
    auto* candidates =
        reinterpret_cast<QVector<Candidate>*>(parameter);

    if (!candidates)
        return TRUE;

    Candidate candidate;

    // Only consider visible, top-level windows.
    if (GetWindow(hwnd, GW_OWNER) != nullptr)
        return TRUE;

    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId == 0)
        return TRUE;

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (!process)
        return TRUE;

    wchar_t path[32768] {};
    DWORD size = 32768;
    const bool queried = QueryFullProcessImageNameW(process, 0, path, &size);
    CloseHandle(process);

    if (!queried)
        return TRUE;

    const QString executable =
        QFileInfo(QString::fromWCharArray(path)).fileName();

    if (executable.compare(QStringLiteral("Photoshop.exe"), Qt::CaseInsensitive) != 0)
        return TRUE;

    candidate.handle = hwnd;
    candidate.processId = processId;

    wchar_t title[512] {};
    const int length = GetWindowTextW(hwnd, title, 512);
    candidate.title = length > 0
        ? QString::fromWCharArray(title, length)
        : QStringLiteral("Adobe Photoshop");

    candidates->append(candidate);
    return TRUE;
}

bool PhotoshopWindowManager::inspectPhotoshopWindow(
    HWND hwnd,
    Candidate& candidate) const
{
    if (!IsWindow(hwnd) || !IsWindowVisible(hwnd))
        return false;

    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);

    if (processId == 0)
        return false;

    HANDLE process =
        OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION,
            FALSE,
            processId);

    if (!process)
        return false;

    wchar_t path[32768] {};
    DWORD size = static_cast<DWORD>(std::size(path));

    const bool queried =
        QueryFullProcessImageNameW(
            process,
            0,
            path,
            &size);

    CloseHandle(process);

    if (!queried)
        return false;

    const QString executable =
        QFileInfo(QString::fromWCharArray(path)).fileName();

    if (executable.compare(
            QStringLiteral("Photoshop.exe"),
            Qt::CaseInsensitive) != 0) {
        return false;
    }

    candidate.handle = hwnd;
    candidate.processId = processId;
    candidate.title = windowTitle(hwnd);
    return true;
}

QString PhotoshopWindowManager::windowTitle(HWND hwnd) const
{
    wchar_t title[512] {};

    const int length =
        GetWindowTextW(hwnd, title, std::size(title));

    if (length <= 0)
        return QStringLiteral("Adobe Photoshop");

    return QString::fromWCharArray(title, length);
}

bool PhotoshopWindowManager::registerHotkeys()
{
    m_hotkeysRegistered = 0;

    for (int i = 0; i < PhotoshopSlotRegistry::kSlotCount; ++i) {
        m_hotkeys[i] =
            RegisterHotKey(
                nullptr,
                kHotkeyBase + i + 1,
                0,
                VK_F1 + i);

        if (m_hotkeys[i])
            ++m_hotkeysRegistered;
    }

    return m_hotkeysRegistered > 0;
}

void PhotoshopWindowManager::unregisterHotkeys()
{
    for (int i = 0; i < PhotoshopSlotRegistry::kSlotCount; ++i) {
        if (m_hotkeys[i])
            UnregisterHotKey(nullptr, kHotkeyBase + i + 1);

        m_hotkeys[i] = false;
    }

    m_hotkeysRegistered = 0;
}

bool PhotoshopWindowManager::activateWindow(HWND hwnd)
{
    if (!IsWindow(hwnd))
        return false;

    if (IsIconic(hwnd))
        ShowWindow(hwnd, SW_RESTORE);

    if (!SetForegroundWindow(hwnd))
        return false;

    return GetForegroundWindow() == hwnd;
}

void PhotoshopWindowManager::syncActiveSlot()
{
    const HWND foreground = GetForegroundWindow();

    const int slot =
        m_registry.slotForHandle(
            reinterpret_cast<quintptr>(foreground));

    if (slot == m_activeSlot)
        return;

    if (m_activeSlot >= 1 &&
        m_activeSlot <= PhotoshopSlotRegistry::kSlotCount) {
        m_registry.at(m_activeSlot).state =
            PhotoshopSlotState::Available;
    }

    m_activeSlot = slot;

    if (m_activeSlot >= 1 &&
        m_activeSlot <= PhotoshopSlotRegistry::kSlotCount) {
        m_registry.at(m_activeSlot).state =
            PhotoshopSlotState::Active;
    }

    emit stateChanged();
}

bool PhotoshopWindowManager::isTrackedWindowValid(
    const PhotoshopSlot& slot) const
{
    return slot.windowHandle != 0 &&
           IsWindow(reinterpret_cast<HWND>(slot.windowHandle));
}

#endif

} // namespace cleanflow

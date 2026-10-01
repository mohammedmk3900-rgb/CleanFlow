#pragma once

#include <QAbstractNativeEventFilter>
#include <QObject>
#include <QStringList>

#include "../../core/PhotoshopSlotRegistry.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace cleanflow {

class PhotoshopWindowManager final
    : public QObject
    , public QAbstractNativeEventFilter
{
    Q_OBJECT
    Q_PROPERTY(int activeSlot READ activeSlot NOTIFY stateChanged)
    Q_PROPERTY(QStringList slotTitles READ slotTitles NOTIFY stateChanged)
    Q_PROPERTY(QStringList slotStates READ slotStates NOTIFY stateChanged)
    Q_PROPERTY(QStringList hotkeyStates READ hotkeyStates NOTIFY stateChanged)
    Q_PROPERTY(bool operational READ operational NOTIFY stateChanged)

public:
    explicit PhotoshopWindowManager(QObject* parent = nullptr);
    ~PhotoshopWindowManager() override;

    int activeSlot() const noexcept { return m_activeSlot; }
    QStringList slotTitles() const;
    QStringList slotStates() const;
    QStringList hotkeyStates() const;
    bool operational() const noexcept { return m_hotkeysRegistered > 0; }

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool switchTo(int slot);

    bool nativeEventFilter(
        const QByteArray& eventType,
        void* message,
        qintptr* result) override;

signals:
    void stateChanged();
    void switchFailed(int slot, const QString& reason);

private:
#ifdef Q_OS_WIN
    struct Candidate {
        HWND handle = nullptr;
        DWORD processId = 0;
        QString title;
    };

    static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM parameter);

    bool inspectPhotoshopWindow(HWND hwnd, Candidate& candidate) const;
    QString windowTitle(HWND hwnd) const;

    bool registerHotkeys();
    void unregisterHotkeys();

    bool activateWindow(HWND hwnd);
    void syncActiveSlot();
    bool isTrackedWindowValid(const PhotoshopSlot& slot) const;

    bool m_hotkeys[PhotoshopSlotRegistry::kSlotCount] {};
#endif

    PhotoshopSlotRegistry m_registry;
    int m_activeSlot = 0;
    int m_hotkeysRegistered = 0;
};

}
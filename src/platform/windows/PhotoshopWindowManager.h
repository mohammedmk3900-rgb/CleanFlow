#pragma once
#include <QObject>
#include <QStringList>
#include <QVector>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
namespace cleanflow {
class PhotoshopWindowManager final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList windows READ windows NOTIFY windowsChanged)
public:
    explicit PhotoshopWindowManager(QObject* parent = nullptr);
    ~PhotoshopWindowManager() override;
    QStringList windows() const;
    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool switchTo(int slot);
#ifdef Q_OS_WIN
    bool handleNativeMessage(MSG* message);
#endif
signals:
    void windowsChanged();
private:
#ifdef Q_OS_WIN
    static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);
    bool isPhotoshopWindow(HWND hwnd) const;
    QString windowTitle(HWND hwnd) const;
    bool registerHotkeys();
    void unregisterHotkeys();
#endif
    QStringList m_windows;
#ifdef Q_OS_WIN
    QVector<HWND> m_handles;
#endif
};
}

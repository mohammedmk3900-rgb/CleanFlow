#include <QtTest>
#include "../src/core/PhotoshopSlotRegistry.h"

using namespace cleanflow;

class PhotoshopSlotRegistryTests final : public QObject {
    Q_OBJECT

private slots:
    void createsFourStableSlots();
    void assignsAndFindsWindow();
    void releasesSlot();
};

void PhotoshopSlotRegistryTests::createsFourStableSlots()
{
    PhotoshopSlotRegistry registry;

    QCOMPARE(registry.slots().size(), 4);
    QCOMPARE(registry.at(1).shortcut, QStringLiteral("F1"));
    QCOMPARE(registry.at(4).shortcut, QStringLiteral("F4"));
    QCOMPARE(registry.firstFreeSlot(), 1);
}

void PhotoshopSlotRegistryTests::assignsAndFindsWindow()
{
    PhotoshopSlotRegistry registry;

    QVERIFY(registry.assign(
        2,
        1234,
        static_cast<quintptr>(0x1234),
        QStringLiteral("Photoshop")));

    QCOMPARE(registry.slotForHandle(static_cast<quintptr>(0x1234)), 2);
    QCOMPARE(registry.at(2).processId, quint64(1234));
    QCOMPARE(registry.at(2).state, PhotoshopSlotState::Available);
}

void PhotoshopSlotRegistryTests::releasesSlot()
{
    PhotoshopSlotRegistry registry;

    QVERIFY(registry.assign(
        3,
        1234,
        static_cast<quintptr>(0x1234),
        QStringLiteral("Photoshop")));

    registry.release(3);

    QCOMPARE(registry.at(3).windowHandle, quintptr(0));
    QCOMPARE(registry.at(3).state, PhotoshopSlotState::Empty);
}

QTEST_MAIN(PhotoshopSlotRegistryTests)
#include "PhotoshopSlotRegistryTests.moc"

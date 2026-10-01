import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 1100
    height: 700
    title: "CleanFlow"

    Column {
        anchors.centerIn: parent
        spacing: 12

        Label {
            text: "CleanFlow"
            font.pixelSize: 32
        }

        Label {
            text: "Parallel Manga & Manhwa Cleaning Workspace"
        }
    }
}

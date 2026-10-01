import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 900
    height: 620
    title: "CleanFlow"
    color: "#101318"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 18

        Label {
            text: "CleanFlow"
            color: "white"
            font.pixelSize: 34
            font.bold: true
        }

        Label {
            text: "Photoshop Switching Workspace"
            color: "#9aa4b2"
            font.pixelSize: 16
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "Refresh Photoshop Windows"
                onClicked: photoshopManager.refresh()
            }

            Item { Layout.fillWidth: true }

            Label {
                text: photoshopManager.windows.length + " detected"
                color: "#9aa4b2"
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            model: photoshopManager.windows

            delegate: Rectangle {
                required property string modelData
                required property int index
                width: ListView.view.width
                height: 76
                radius: 10
                color: "#191e26"
                border.color: "#2a3340"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 16

                    Label {
                        text: "F" + (index + 1)
                        color: "white"
                        font.pixelSize: 22
                        font.bold: true
                        Layout.preferredWidth: 44
                    }

                    ColumnLayout {
                        Layout.fillWidth: true

                        Label {
                            text: modelData
                            color: "white"
                            font.pixelSize: 15
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        Label {
                            text: "Photoshop workspace"
                            color: "#7f8a99"
                            font.pixelSize: 12
                        }
                    }

                    Button {
                        text: "Switch"
                        onClicked: photoshopManager.switchTo(index + 1)
                    }
                }
            }

            Label {
                anchors.centerIn: parent
                visible: photoshopManager.windows.length === 0
                text: "No visible Photoshop windows detected."
                color: "#7f8a99"
            }
        }

        Label {
            text: "Global hotkeys: F1-F4"
            color: "#7f8a99"
        }
    }
}

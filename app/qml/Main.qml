import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 980
    height: 700
    minimumWidth: 760
    minimumHeight: 560
    title: "CleanFlow"
    color: "#0d1117"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 18

        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 3

                Label {
                    text: "CleanFlow"
                    color: "#f0f3f6"
                    font.pixelSize: 32
                    font.bold: true
                }

                Label {
                    text: "Photoshop Switching Workspace"
                    color: "#8b949e"
                    font.pixelSize: 15
                }
            }

            ColumnLayout {
                spacing: 3
                Label {
                    text: photoshopManager.operational ? "OPERATIONAL" : "HOTKEYS DEGRADED"
                    color: photoshopManager.operational ? "#3fb950" : "#d29922"
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    text: photoshopManager.activeSlot > 0
                          ? "Active: F" + photoshopManager.activeSlot
                          : "Active: none"
                    color: "#8b949e"
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Refresh"
                onClicked: photoshopManager.refresh()
            }

            Label {
                text: "F1–F4 switch Photoshop slots globally"
                color: "#8b949e"
                Layout.fillWidth: true
            }
        }

        ListView {
            id: slotsView
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            clip: true
            model: 4

            delegate: Rectangle {
                required property int index

                readonly property int slotNumber: index + 1
                readonly property string slotTitle: photoshopManager.slotTitles[index] || ""
                readonly property string slotState: photoshopManager.slotStates[index] || "Empty"
                readonly property string hotkeyState: photoshopManager.hotkeyStates[index] || "Unavailable"
                readonly property bool active: photoshopManager.activeSlot === slotNumber
                readonly property bool available: slotTitle.length > 0 && slotState !== "Empty"

                width: ListView.view.width
                height: 92
                radius: 12
                color: active ? "#17251b" : "#151b23"
                border.width: active ? 2 : 1
                border.color: active ? "#3fb950" : "#28313d"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    Rectangle {
                        Layout.preferredWidth: 58
                        Layout.preferredHeight: 58
                        radius: 10
                        color: active ? "#238636" : "#21262d"

                        Label {
                            anchors.centerIn: parent
                            text: "F" + slotNumber
                            color: "#f0f3f6"
                            font.pixelSize: 20
                            font.bold: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: available ? slotTitle : "No Photoshop instance detected"
                            color: available ? "#f0f3f6" : "#8b949e"
                            font.pixelSize: 15
                            font.bold: available
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        RowLayout {
                            spacing: 12

                            Label {
                                text: slotState
                                color: active ? "#3fb950" : "#8b949e"
                                font.pixelSize: 12
                            }

                            Label {
                                text: hotkeyState
                                color: hotkeyState === "Registered" ? "#3fb950" : "#d29922"
                                font.pixelSize: 12
                            }
                        }
                    }

                    Button {
                        text: active ? "Active" : "Switch"
                        enabled: available && !active
                        onClicked: photoshopManager.switchTo(slotNumber)
                    }
                }
            }
        }

        Label {
            Layout.fillWidth: true
            text: "CleanFlow never edits Photoshop documents. It only discovers, tracks, and switches windows."
            color: "#6e7681"
            font.pixelSize: 12
            wrapMode: Text.WordWrap
        }
    }
}

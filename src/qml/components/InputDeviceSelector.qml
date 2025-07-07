import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: inputDeviceSelector
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 6

    property string selectedDevice: "spacemouse"

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Text {
            text: "🎮 Input Device"
            color: "#333333"
            font.pixelSize: 12
            font.bold: true
        }

        Column {
            width: parent.width
            spacing: 6

            // SpaceMouse Option
            Rectangle {
                width: parent.width
                height: 30
                color: inputDeviceSelector.selectedDevice === "spacemouse" ? "#4A90E2" : "#F8F9FA"
                radius: 4
                border.color: "#4A90E2"
                border.width: 1

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        border.color: "#4A90E2"
                        border.width: 1
                        color: inputDeviceSelector.selectedDevice === "spacemouse" ? "#4A90E2" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: inputDeviceSelector.selectedDevice === "spacemouse"
                        }
                    }

                    Text {
                        text: "🚀 SpaceMouse"
                        color: inputDeviceSelector.selectedDevice === "spacemouse" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        inputDeviceSelector.selectedDevice = "spacemouse"
                        console.log("SpaceMouse selected")
                    }
                }
            }

            // Trackpad Option
            Rectangle {
                width: parent.width
                height: 30
                color: inputDeviceSelector.selectedDevice === "trackpad" ? "#9C27B0" : "#F8F9FA"
                radius: 4
                border.color: "#9C27B0"
                border.width: 1

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        border.color: "#9C27B0"
                        border.width: 1
                        color: inputDeviceSelector.selectedDevice === "trackpad" ? "#9C27B0" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: inputDeviceSelector.selectedDevice === "trackpad"
                        }
                    }

                    Text {
                        text: "👆 Trackpad"
                        color: inputDeviceSelector.selectedDevice === "trackpad" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        inputDeviceSelector.selectedDevice = "trackpad"
                        console.log("Trackpad selected")
                    }
                }
            }

            // Mouse Option
            Rectangle {
                width: parent.width
                height: 30
                color: inputDeviceSelector.selectedDevice === "mouse" ? "#757575" : "#F8F9FA"
                radius: 4
                border.color: "#757575"
                border.width: 1

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        border.color: "#757575"
                        border.width: 1
                        color: inputDeviceSelector.selectedDevice === "mouse" ? "#757575" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: inputDeviceSelector.selectedDevice === "mouse"
                        }
                    }

                    Text {
                        text: "🖱️ Mouse"
                        color: inputDeviceSelector.selectedDevice === "mouse" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        inputDeviceSelector.selectedDevice = "mouse"
                        console.log("Mouse selected")
                    }
                }
            }
        }
    }
}

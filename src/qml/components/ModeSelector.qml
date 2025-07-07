import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: modeSelector
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 8

    property string selectedMode: "groundTruth"

    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Text {
            text: "Operation Mode"
            color: "#333333"
            font.pixelSize: 18
            font.bold: true
        }

        Column {
            width: parent.width
            spacing: 12

            // Ground Truth Mode
            Rectangle {
                width: parent.width
                height: 40
                color: modeSelector.selectedMode === "groundTruth" ? "#4A90E2" : "#E8F4FF"
                radius: 8
                border.color: "#4A90E2"
                border.width: 1

                Row {
                    anchors.centerIn: parent
                    spacing: 8

                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        border.color: "#4A90E2"
                        border.width: 2
                        color: modeSelector.selectedMode === "groundTruth" ? "#4A90E2" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: modeSelector.selectedMode === "groundTruth"
                        }
                    }

                    Text {
                        text: "Ground Truth Mode"
                        color: modeSelector.selectedMode === "groundTruth" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 14
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: modeSelector.selectedMode = "groundTruth"
                }
            }

            // Stereo Mode
            Rectangle {
                width: parent.width
                height: 40
                color: modeSelector.selectedMode === "stereo" ? "#4A90E2" : "#E8F4FF"
                radius: 8
                border.color: "#4A90E2"
                border.width: 1

                Row {
                    anchors.centerIn: parent
                    spacing: 8

                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        border.color: "#4A90E2"
                        border.width: 2
                        color: modeSelector.selectedMode === "stereo" ? "#4A90E2" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: modeSelector.selectedMode === "stereo"
                        }
                    }

                    Text {
                        text: "Stereo Image Mode"
                        color: modeSelector.selectedMode === "stereo" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 14
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: modeSelector.selectedMode = "stereo"
                }
            }
        }
    }
}
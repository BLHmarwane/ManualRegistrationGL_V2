import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: modeSelector
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 6

    property string selectedMode: "groundTruth"

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Text {
            text: "📷 Operation Mode"
            color: "#333333"
            font.pixelSize: 12
            font.bold: true
        }

        Column {
            width: parent.width
            spacing: 6

            // Ground Truth Mode
            Rectangle {
                width: parent.width
                height: 30
                color: modeSelector.selectedMode === "groundTruth" ? "#4A90E2" : "#F8F9FA"
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
                        color: modeSelector.selectedMode === "groundTruth" ? "#4A90E2" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: modeSelector.selectedMode === "groundTruth"
                        }
                    }

                    Text {
                        text: " Ground Truth"
                        color: modeSelector.selectedMode === "groundTruth" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
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
                height: 30
                color: modeSelector.selectedMode === "stereo" ? "#4A90E2" : "#F8F9FA"
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
                        color: modeSelector.selectedMode === "stereo" ? "#4A90E2" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: modeSelector.selectedMode === "stereo"
                        }
                    }



                    Text {
                        text: " Stereo Image"
                        color: modeSelector.selectedMode === "stereo" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
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

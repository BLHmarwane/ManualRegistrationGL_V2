import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: transformDisplay
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 8

    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Text {
            text: "Live Transform"
            color: "#333333"
            font.pixelSize: 18
            font.bold: true
        }

        // Translation
        Column {
            width: parent.width
            spacing: 8

            Text {
                text: "Translation"
                font.pixelSize: 14
                font.bold: true
                color: "#333333"
            }

            Rectangle {
                width: parent.width
                height: 60
                color: "#E8F4FF"
                radius: 8

                Grid {
                    anchors.centerIn: parent
                    columns: 2
                    rowSpacing: 4
                    columnSpacing: 12

                    Text { text: "X:"; font.pixelSize: 12; color: "#333333" }
                    Text { text: "0.00"; font.family: "monospace"; font.pixelSize: 12; color: "#4A90E2"; font.bold: true }
                    Text { text: "Y:"; font.pixelSize: 12; color: "#333333" }
                    Text { text: "-5.00"; font.family: "monospace"; font.pixelSize: 12; color: "#4A90E2"; font.bold: true }
                    Text { text: "Z:"; font.pixelSize: 12; color: "#333333" }
                    Text { text: "0.00"; font.family: "monospace"; font.pixelSize: 12; color: "#4A90E2"; font.bold: true }
                }
            }
        }

        // Rotation
        Column {
            width: parent.width
            spacing: 8

            Text {
                text: "Rotation (degrees)"
                font.pixelSize: 14
                font.bold: true
                color: "#333333"
            }

            Rectangle {
                width: parent.width
                height: 60
                color: "#E8F4FF"
                radius: 8

                Grid {
                    anchors.centerIn: parent
                    columns: 2
                    rowSpacing: 4
                    columnSpacing: 12

                    Text { text: "X:"; font.pixelSize: 12; color: "#333333" }
                    Text { text: "0.0°"; font.family: "monospace"; font.pixelSize: 12; color: "#4A90E2"; font.bold: true }
                    Text { text: "Y:"; font.pixelSize: 12; color: "#333333" }
                    Text { text: "0.0°"; font.family: "monospace"; font.pixelSize: 12; color: "#4A90E2"; font.bold: true }
                    Text { text: "Z:"; font.pixelSize: 12; color: "#333333" }
                    Text { text: "0.0°"; font.family: "monospace"; font.pixelSize: 12; color: "#4A90E2"; font.bold: true }
                }
            }
        }

        // Scale
        Column {
            width: parent.width
            spacing: 8

            Text {
                text: "Scale"
                font.pixelSize: 14
                font.bold: true
                color: "#333333"
            }

            Rectangle {
                width: parent.width
                height: 30
                color: "#E8F4FF"
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: "1.00"
                    font.family: "monospace"
                    font.pixelSize: 14
                    color: "#4A90E2"
                    font.bold: true
                }
            }
        }

        // SpaceMouse Status
        Rectangle {
            width: parent.width
            height: 40
            color: "#4A90E2"
            radius: 8

            Row {
                anchors.centerIn: parent
                spacing: 8

                Rectangle {
                    width: 8
                    height: 8
                    radius: 4
                    color: "#4CAF50"

                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.3; duration: 1000 }
                        NumberAnimation { to: 1.0; duration: 1000 }
                    }
                }

                Text {
                    text: "SpaceMouse Ready"
                    font.pixelSize: 12
                    color: "#FFFFFF"
                    font.bold: true
                }
            }
        }
    }
}
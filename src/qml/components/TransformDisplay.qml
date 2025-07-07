import QtQuick 2.15

Rectangle {
    id: transformDisplay
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 6

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        Text {
            text: "📊 Live Transform"
            color: "#333333"
            font.pixelSize: 12
            font.bold: true
        }

        // Translation
        Column {
            width: parent.width
            spacing: 4

            Text {
                text: "Translation"
                font.pixelSize: 11
                font.bold: true
                color: "#333333"
            }

            Rectangle {
                width: parent.width
                height: 45
                color: "#E8F4FF"
                radius: 4

                Grid {
                    anchors.centerIn: parent
                    columns: 2
                    rowSpacing: 2
                    columnSpacing: 8

                    Text { text: "X:"; font.pixelSize: 10; color: "#333333" }
                    Text { text: "0.00"; font.family: "monospace"; font.pixelSize: 10; color: "#4A90E2"; font.bold: true }
                    Text { text: "Y:"; font.pixelSize: 10; color: "#333333" }
                    Text { text: "-5.00"; font.family: "monospace"; font.pixelSize: 10; color: "#4A90E2"; font.bold: true }
                    Text { text: "Z:"; font.pixelSize: 10; color: "#333333" }
                    Text { text: "0.00"; font.family: "monospace"; font.pixelSize: 10; color: "#4A90E2"; font.bold: true }
                }
            }
        }

        // Rotation
        Column {
            width: parent.width
            spacing: 4

            Text {
                text: "Rotation (degrees)"
                font.pixelSize: 11
                font.bold: true
                color: "#333333"
            }

            Rectangle {
                width: parent.width
                height: 45
                color: "#E8F4FF"
                radius: 4

                Grid {
                    anchors.centerIn: parent
                    columns: 2
                    rowSpacing: 2
                    columnSpacing: 8

                    Text { text: "X:"; font.pixelSize: 10; color: "#333333" }
                    Text { text: "0.0°"; font.family: "monospace"; font.pixelSize: 10; color: "#4A90E2"; font.bold: true }
                    Text { text: "Y:"; font.pixelSize: 10; color: "#333333" }
                    Text { text: "0.0°"; font.family: "monospace"; font.pixelSize: 10; color: "#4A90E2"; font.bold: true }
                    Text { text: "Z:"; font.pixelSize: 10; color: "#333333" }
                    Text { text: "0.0°"; font.family: "monospace"; font.pixelSize: 10; color: "#4A90E2"; font.bold: true }
                }
            }
        }

        // Scale
        Column {
            width: parent.width
            spacing: 4

            Text {
                text: "Scale"
                font.pixelSize: 11
                font.bold: true
                color: "#333333"
            }

            Rectangle {
                width: parent.width
                height: 25
                color: "#E8F4FF"
                radius: 4

                Text {
                    anchors.centerIn: parent
                    text: "1.00"
                    font.family: "monospace"
                    font.pixelSize: 11
                    color: "#4A90E2"
                    font.bold: true
                }
            }
        }
    }
}

import QtQuick 2.15

Rectangle {
    id: viewport
    color: "#1a1a1a"
    radius: 8

    // Placeholder for 3D content
    Rectangle {
        anchors.centerIn: parent
        width: 300
        height: 300
        color: "transparent"
        border.color: "#4A90E2"
        border.width: 2
        radius: 150

        // Animated rotating element to show it's "3D"
        Rectangle {
            id: shape
            anchors.centerIn: parent
            width: 120
            height: 120
            color: "#4A90E2"
            opacity: 0.7
            radius: 8

            RotationAnimation {
                target: shape
                property: "rotation"
                from: 0
                to: 360
                duration: 4000
                loops: Animation.Infinite
                running: true
            }
        }

        // Corner labels to show axes
        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            text: "3D Viewport"
            color: "#4A90E2"
            font.pixelSize: 14
            font.bold: true
        }
    }

    // Status overlay
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 16
        width: 120
        height: 30
        color: "#4A90E2"
        radius: 15
        opacity: 0.9

        Text {
            anchors.centerIn: parent
            text: "Ready"
            color: "#FFFFFF"
            font.pixelSize: 12
            font.bold: true
        }
    }
}
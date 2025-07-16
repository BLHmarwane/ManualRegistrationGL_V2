import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: inputModeSelector
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 6

    // Reference to the 3D viewport - this will be set by the parent
    property var viewport3D: null

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        Text {
            text: "🎮 Interaction Mode"
            color: "#333333"
            font.pixelSize: 14
            font.bold: true
        }

        // Current Mode Display
        Rectangle {
            width: parent.width
            height: 40
            color: "#E8F4FD"
            radius: 6
            border.color: "#4A90E2"
            border.width: 2

            Column {
                anchors.centerIn: parent
                spacing: 2

                Text {
                    text: "Current Mode:"
                    font.pixelSize: 10
                    color: "#666666"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    id: currentModeText
                    text: getCurrentModeDisplayText()
                    font.pixelSize: 12
                    font.bold: true
                    color: "#2E3440"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        // Mode Selection Buttons
        Column {
            width: parent.width
            spacing: 8

            // Mouse Mode
            Rectangle {
                width: parent.width
                height: 35
                color: (viewport3D && viewport3D.interactionMode === "Mouse") ? "#4A90E2" : "#F8F9FA"
                radius: 6
                border.color: "#4A90E2"
                border.width: 1

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        border.color: "#4A90E2"
                        border.width: 1
                        color: (viewport3D && viewport3D.interactionMode === "Mouse") ? "#4A90E2" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: viewport3D && viewport3D.interactionMode === "Mouse"
                        }
                    }

                    Text {
                        text: "🖱️ Traditional Mouse (2D)"
                        color: (viewport3D && viewport3D.interactionMode === "Mouse") ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (viewport3D) {
                            viewport3D.interactionMode = "Mouse"
                        }
                        console.log("Mouse mode selected")
                    }
                }
            }

            // SpaceMouse Mode
            Rectangle {
                width: parent.width
                height: 35
                color: (viewport3D && viewport3D.interactionMode === "SpaceMouse") ? "#FF6B35" : "#F8F9FA"
                radius: 6
                border.color: (viewport3D && viewport3D.spaceMouseConnected) ? "#FF6B35" : "#CCCCCC"
                border.width: 1
                opacity: (viewport3D && viewport3D.spaceMouseConnected) ? 1.0 : 0.6

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        border.color: (viewport3D && viewport3D.spaceMouseConnected) ? "#FF6B35" : "#CCCCCC"
                        border.width: 1
                        color: (viewport3D && viewport3D.interactionMode === "SpaceMouse") ? "#FF6B35" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: viewport3D && viewport3D.interactionMode === "SpaceMouse"
                        }
                    }

                    Text {
                        text: (viewport3D && viewport3D.spaceMouseConnected) ? "🚀 SpaceMouse (6DOF)" : "🚀 SpaceMouse (Not Connected)"
                        color: (viewport3D && viewport3D.interactionMode === "SpaceMouse") ? "#FFFFFF" :
                               (viewport3D && viewport3D.spaceMouseConnected) ? "#333333" : "#999999"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: viewport3D && viewport3D.spaceMouseConnected
                    onClicked: {
                        if (viewport3D) {
                            viewport3D.interactionMode = "SpaceMouse"
                        }
                        console.log("SpaceMouse mode selected")
                    }
                }

                // Connection status indicator
                Rectangle {
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    width: 8
                    height: 8
                    radius: 4
                    color: (viewport3D && viewport3D.spaceMouseConnected) ? "#4CAF50" : "#F44336"
                }
            }

            // MultiTouch Mode (Future Implementation)
            Rectangle {
                width: parent.width
                height: 35
                color: "#F8F9FA"
                radius: 6
                border.color: "#CCCCCC"
                border.width: 1
                opacity: 0.6

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        border.color: "#CCCCCC"
                        border.width: 1
                        color: "transparent"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: "👆 Multi-Touch (Coming Soon)"
                        color: "#999999"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }

    // Helper function to get display text for current mode
    function getCurrentModeDisplayText() {
        if (!viewport3D) return "No Connection"

        switch(viewport3D.interactionMode) {
            case "Mouse":
                return "Traditional Mouse (2D)"
            case "SpaceMouse":
                return viewport3D.spaceMouseConnected ? "SpaceMouse (6DOF)" : "SpaceMouse (Disconnected)"
            case "MultiTouch":
                return "Multi-Touch Trackpad"
            default:
                return "Unknown Mode"
        }
    }

    // Update current mode text when interaction mode changes
    Connections {
        target: viewport3D
        function onInteractionModeChanged() {
            currentModeText.text = getCurrentModeDisplayText()
        }
        function onSpaceMouseConnectionChanged() {
            currentModeText.text = getCurrentModeDisplayText()
        }
    }
}
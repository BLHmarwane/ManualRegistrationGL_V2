import QtQuick 2.15

Rectangle {
    id: mainWindow
    color: "#F8F9FA"

    Row {
        anchors.fill: parent
        spacing: 4

        // Left Control Panel (25% width)
        Rectangle {
            width: parent.width * 0.25
            height: parent.height
            color: "#FFFFFF"
            border.color: "#E1E5E9"
            border.width: 1

            Column {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                // Header
                Rectangle {
                    width: parent.width
                    height: 50
                    color: "#2E3440"
                    radius: 6

                    Text {
                        anchors.centerIn: parent
                        text: "SURGAR V2"
                        color: "#FFFFFF"
                        font.pixelSize: 16
                        font.bold: true
                        font.family: "Arial"
                    }
                }

                // Operation Mode Section - FIXED with Load Model button
                Rectangle {
                    width: parent.width
                    height: 160
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Operation Mode"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#4A90E2"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Ground Truth Mode"
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Ground Truth mode selected")
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#F8F9FA"
                            radius: 6
                            border.color: "#4A90E2"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Stereo Image Mode"
                                color: "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Stereo mode selected")
                            }
                        }

                        // Load Custom Model Button - NOW CONNECTED TO STEREO MODE
                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#607D8B"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Load Custom Model..."
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Load model clicked")
                            }
                        }
                    }
                }

                // Input Device Section
                Rectangle {
                    width: parent.width
                    height: 180
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Input Device"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        // SpaceMouse Button
                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#4A90E2"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "SpaceMouse (6DOF)"
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("SpaceMouse selected")
                            }
                        }

                        // Trackpad Button
                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#F8F9FA"
                            radius: 6
                            border.color: "#9C27B0"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Trackpad (Multi-touch)"
                                color: "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Trackpad selected")
                            }
                        }

                        // Mouse Button
                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#F8F9FA"
                            radius: 6
                            border.color: "#757575"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Mouse (Traditional)"
                                color: "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Mouse selected")
                            }
                        }
                    }
                }

                // Shape Selection Section - REMOVED Load Model button from here
                Rectangle {
                    width: parent.width
                    height: 180
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "3D Shape"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#F8F9FA"
                            radius: 6
                            border.color: "#FF5722"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Cube"
                                color: "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Cube selected")
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#F8F9FA"
                            radius: 6
                            border.color: "#2196F3"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Sphere"
                                color: "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Sphere selected")
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#F8F9FA"
                            radius: 6
                            border.color: "#4CAF50"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Torus"
                                color: "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Torus selected")
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#9C27B0"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Tetrahedron"
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Tetrahedron selected")
                            }
                        }
                    }
                }
            }
        }

        // Main 3D Viewport (50% width)
        Rectangle {
            width: parent.width * 0.5
            height: parent.height
            color: "#F5F5F5"
            border.color: "#E1E5E9"
            border.width: 2

            // 3D Viewport content
            Rectangle {
                anchors.centerIn: parent
                width: 320
                height: 320
                color: "transparent"
                border.color: "#4A90E2"
                border.width: 3
                radius: 160

                // Animated rotating element
                Rectangle {
                    id: shape
                    anchors.centerIn: parent
                    width: 140
                    height: 140
                    color: "#4A90E2"
                    opacity: 0.8
                    radius: 10

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

                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 12
                    text: "3D Viewport"
                    color: "#4A90E2"
                    font.pixelSize: 18
                    font.bold: true
                    font.family: "Arial"
                }
            }

            // Status overlay
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 20
                width: 100
                height: 32
                color: "#4A90E2"
                radius: 16
                opacity: 0.9

                Text {
                    anchors.centerIn: parent
                    text: "Ready"
                    color: "#FFFFFF"
                    font.pixelSize: 12
                    font.bold: true
                    font.family: "Arial"
                }
            }
        }

        // Right Control Panel (25% width)
        Rectangle {
            width: parent.width * 0.25
            height: parent.height
            color: "#FFFFFF"
            border.color: "#E1E5E9"
            border.width: 1

            Column {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                // Session Controls Section
                Rectangle {
                    width: parent.width
                    height: 180
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Session Control"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        Rectangle {
                            width: parent.width
                            height: 38
                            color: "#4CAF50"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Start Session"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#FFFFFF"
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Start session clicked")
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 38
                            color: "#FF9800"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Reset Transform"
                                font.pixelSize: 12
                                color: "#FFFFFF"
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Reset clicked")
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 38
                            color: "#F44336"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Finish Session"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#FFFFFF"
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Finish clicked")
                            }
                        }
                    }
                }

                // Transform Display Section
                Rectangle {
                    width: parent.width
                    height: 260
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Live Transform"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        // Translation
                        Column {
                            width: parent.width
                            spacing: 6

                            Text {
                                text: "Translation"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#2E3440"
                                font.family: "Arial"
                            }

                            Rectangle {
                                width: parent.width
                                height: 45
                                color: "#F8F9FA"
                                radius: 6
                                border.color: "#E1E5E9"
                                border.width: 1

                                Column {
                                    anchors.centerIn: parent
                                    spacing: 2

                                    Text {
                                        text: "X: 0.00  Y: -5.00  Z: 0.00"
                                        font.family: "Consolas, Monaco, monospace"
                                        font.pixelSize: 11
                                        color: "#4A90E2"
                                        font.bold: true
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                }
                            }
                        }

                        // Rotation
                        Column {
                            width: parent.width
                            spacing: 6

                            Text {
                                text: "Rotation (degrees)"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#2E3440"
                                font.family: "Arial"
                            }

                            Rectangle {
                                width: parent.width
                                height: 45
                                color: "#F8F9FA"
                                radius: 6
                                border.color: "#E1E5E9"
                                border.width: 1

                                Text {
                                    anchors.centerIn: parent
                                    text: "X: 0.0°  Y: 0.0°  Z: 0.0°"
                                    font.family: "Consolas, Monaco, monospace"
                                    font.pixelSize: 11
                                    color: "#4A90E2"
                                    font.bold: true
                                }
                            }
                        }

                        // Scale
                        Column {
                            width: parent.width
                            spacing: 6

                            Text {
                                text: "Scale"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#2E3440"
                                font.family: "Arial"
                            }

                            Rectangle {
                                width: parent.width
                                height: 30
                                color: "#F8F9FA"
                                radius: 6
                                border.color: "#E1E5E9"
                                border.width: 1

                                Text {
                                    anchors.centerIn: parent
                                    text: "1.00"
                                    font.family: "Consolas, Monaco, monospace"
                                    font.pixelSize: 12
                                    color: "#4A90E2"
                                    font.bold: true
                                }
                            }
                        }
                    }
                }

                // Device Status Section
                Rectangle {
                    width: parent.width
                    height: 50
                    color: "#E8F4FD"
                    radius: 8
                    border.color: "#4A90E2"
                    border.width: 1

                    Column {
                        anchors.centerIn: parent
                        spacing: 4

                        Text {
                            text: "SpaceMouse"
                            font.pixelSize: 12
                            font.bold: true
                            color: "#2E3440"
                            font.family: "Arial"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            text: "Ready"
                            font.pixelSize: 10
                            color: "#4A90E2"
                            font.family: "Arial"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
        }
    }
}

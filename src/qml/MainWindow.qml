import QtQuick 2.15
import SURGAR.Components 1.0

Rectangle {
    id: mainWindow
    color: "#F8F9FA"

    // 3D Viewport reference
    property alias viewport3D: viewport3D

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

                // Operation Mode Section
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

               // Shape Selection Section - CONNECTED TO 3D RENDERER!
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
                           color: viewport3D.currentShape === 1 ? "#FF5722" : "#F8F9FA"
                           radius: 6
                           border.color: "#FF5722"
                           border.width: 1

                           Text {
                               anchors.centerIn: parent
                               text: "Cube"
                               color: viewport3D.currentShape === 1 ? "#FFFFFF" : "#2E3440"
                               font.pixelSize: 12
                               font.family: "Arial"
                           }

                           MouseArea {
                               anchors.fill: parent
                               onClicked: {
                                   viewport3D.currentShape = 1
                                   console.log("Cube selected")
                               }
                           }
                       }

                       Rectangle {
                           width: parent.width
                           height: 32
                           color: viewport3D.currentShape === 2 ? "#2196F3" : "#F8F9FA"
                           radius: 6
                           border.color: "#2196F3"
                           border.width: 1

                           Text {
                               anchors.centerIn: parent
                               text: "Sphere"
                               color: viewport3D.currentShape === 2 ? "#FFFFFF" : "#2E3440"
                               font.pixelSize: 12
                               font.family: "Arial"
                           }

                           MouseArea {
                               anchors.fill: parent
                               onClicked: {
                                   viewport3D.currentShape = 2
                                   console.log("Sphere selected")
                               }
                           }
                       }

                       Rectangle {
                           width: parent.width
                           height: 32
                           color: viewport3D.currentShape === 3 ? "#4CAF50" : "#F8F9FA"
                           radius: 6
                           border.color: "#4CAF50"
                           border.width: 1

                           Text {
                               anchors.centerIn: parent
                               text: "Torus"
                               color: viewport3D.currentShape === 3 ? "#FFFFFF" : "#2E3440"
                               font.pixelSize: 12
                               font.family: "Arial"
                           }

                           MouseArea {
                               anchors.fill: parent
                               onClicked: {
                                   viewport3D.currentShape = 3
                                   console.log("Torus selected")
                               }
                           }
                       }

                       Rectangle {
                           width: parent.width
                           height: 32
                           color: viewport3D.currentShape === 4 ? "#9C27B0" : "#F8F9FA"
                           radius: 6
                           border.color: "#9C27B0"
                           border.width: 1

                           Text {
                               anchors.centerIn: parent
                               text: "Tetrahedron"
                               color: viewport3D.currentShape === 4 ? "#FFFFFF" : "#2E3440"
                               font.pixelSize: 12
                               font.family: "Arial"
                           }

                           MouseArea {
                               anchors.fill: parent
                               onClicked: {
                                   viewport3D.currentShape = 4
                                   console.log("Tetrahedron selected")
                               }
                           }
                       }
                   }
               }
           }
       }

       // Main 3D Viewport (50% width) - REAL OPENGL RENDERER!
       Rectangle {
           width: parent.width * 0.5
           height: parent.height
           color: "#F5F5F5"
           border.color: "#E1E5E9"
           border.width: 2
           radius: 8

           OpenGL3DViewport {
               id: viewport3D
               anchors.fill: parent
               anchors.margins: 4

               // Default values
               currentShape: 4  // Tetrahedron
               translation: Qt.vector3d(0.0, 0.0, -5.0)
               rotation: Qt.vector3d(0.0, 0.0, 0.0)
               scale: 1.0

               // Connect transform changes to update display
               onTransformChanged: {
                   translationDisplay.updateValues()
                   rotationDisplay.updateValues()
                   scaleDisplay.updateValues()
               }
           }

           // 3D Viewport Label
           Text {
               anchors.top: parent.top
               anchors.left: parent.left
               anchors.margins: 12
               text: "3D Viewport - OpenGL Renderer"
               color: "#4A90E2"
               font.pixelSize: 14
               font.bold: true
               font.family: "Arial"
           }

           // Status overlay
           Rectangle {
               anchors.bottom: parent.bottom
               anchors.right: parent.right
               anchors.margins: 20
               width: 120
               height: 32
               color: "#4A90E2"
               radius: 16
               opacity: 0.9

               Text {
                   anchors.centerIn: parent
                   text: "OpenGL Active"
                   color: "#FFFFFF"
                   font.pixelSize: 11
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
                               onClicked: {
                                   console.log("Start session clicked")
                                   // Apply demo transform
                                   viewport3D.translation = Qt.vector3d(-2.0, 1.0, -6.0)
                                   viewport3D.rotation = Qt.vector3d(30.0, 45.0, 0.0)
                                   viewport3D.scale = 1.5
                               }
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
                               onClicked: {
                                   console.log("Reset clicked")
                                   viewport3D.resetTransform()
                               }
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

               // Transform Display Section - REAL-TIME VALUES!
               Rectangle {
                   id: transformDisplay
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
                           id: translationDisplay
                           width: parent.width
                           spacing: 6

                           function updateValues() {
                               translationText.text = "X: " + viewport3D.translation.x.toFixed(2) +
                                                    "  Y: " + viewport3D.translation.y.toFixed(2) +
                                                    "  Z: " + viewport3D.translation.z.toFixed(2)
                           }

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

                               Text {
                                   id: translationText
                                   anchors.centerIn: parent
                                   text: "X: " + viewport3D.translation.x.toFixed(2) +
                                         "  Y: " + viewport3D.translation.y.toFixed(2) +
                                         "  Z: " + viewport3D.translation.z.toFixed(2)
                                   font.family: "Consolas, Monaco, monospace"
                                   font.pixelSize: 11
                                   color: "#4A90E2"
                                   font.bold: true
                               }
                           }
                       }

                       // Rotation
                       Column {
                           id: rotationDisplay
                           width: parent.width
                           spacing: 6

                           function updateValues() {
                               rotationText.text = "X: " + viewport3D.rotation.x.toFixed(1) + "°" +
                                                 "  Y: " + viewport3D.rotation.y.toFixed(1) + "°" +
                                                 "  Z: " + viewport3D.rotation.z.toFixed(1) + "°"
                           }

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
                                   id: rotationText
                                   anchors.centerIn: parent
                                   text: "X: " + viewport3D.rotation.x.toFixed(1) + "°" +
                                         "  Y: " + viewport3D.rotation.y.toFixed(1) + "°" +
                                         "  Z: " + viewport3D.rotation.z.toFixed(1) + "°"
                                   font.family: "Consolas, Monaco, monospace"
                                   font.pixelSize: 11
                                   color: "#4A90E2"
                                   font.bold: true
                               }
                           }
                       }

                       // Scale
                       Column {
                           id: scaleDisplay
                           width: parent.width
                           spacing: 6

                           function updateValues() {
                               scaleText.text = viewport3D.scale.toFixed(2)
                           }

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
                                   id: scaleText
                                   anchors.centerIn: parent
                                   text: viewport3D.scale.toFixed(2)
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
                           text: "OpenGL Renderer"
                           font.pixelSize: 12
                           font.bold: true
                           color: "#2E3440"
                           font.family: "Arial"
                           anchors.horizontalCenter: parent.horizontalCenter
                       }

                       Text {
                           text: "Active"
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

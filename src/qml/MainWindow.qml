import QtQuick 2.15
import SURGAR.Components 1.0

// =================================================================
// MAIN WINDOW - SURGAR V2 RESEARCH SIMULATOR
// =================================================================
// Main container for the dual-model 3D registration research interface
// Layout: 25% left panel + 50% 3D viewport + 25% right panel
Rectangle {
    id: mainWindow
    color: "#F8F9FA"

    // 3D Viewport reference for easy access throughout the interface
    property alias viewport3D: viewport3D

    // =================================================================
    // MAIN LAYOUT - THREE COLUMN DESIGN
    // =================================================================
    Row {
        anchors.fill: parent
        spacing: 4

        // =================================================================
        // LEFT CONTROL PANEL (25% width)
        // =================================================================
        // Contains: Operation Mode, Input Device Selection, Shape Selection
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

                // -----------------------------------------------------
                // HEADER SECTION
                // -----------------------------------------------------
                Rectangle {
                    width: parent.width
                    height: 50
                    color: "#2E3440"
                    radius: 6

                    Text {
                        anchors.centerIn: parent
                        text: "SURGAR V2 Research"
                        color: "#FFFFFF"
                        font.pixelSize: 14
                        font.bold: true
                        font.family: "Arial"
                    }
                }

                // -----------------------------------------------------
                // OPERATION MODE SECTION
                // -----------------------------------------------------
                // Selection between different research modes
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

                        // Dual Model Alignment Mode (Active)
                        Rectangle {
                            width: parent.width
                            height: 32
                            color: "#4A90E2"
                            radius: 6

                            Text {
                                anchors.centerIn: parent
                                text: "Dual Model Alignment"
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Dual model mode selected")
                            }
                        }

                        // Stereo Image Mode (Future Implementation)
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

                        // Load Custom Model Option
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

                // -----------------------------------------------------
                // INPUT DEVICE SELECTION SECTION
                // -----------------------------------------------------
                // SpaceMouse, Trackpad, and Mouse options
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

                        // SpaceMouse Option (6DOF Professional Input)
                        Rectangle {
                            width: parent.width
                            height: 32
                            // Dynamic color based on current interaction mode
                            color: viewport3D.interactionMode === "SpaceMouse" ? "#4A90E2" : "#F8F9FA"
                            radius: 6
                            border.color: "#4A90E2"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "SpaceMouse (6DOF)"
                                // Dynamic text color based on selection
                                color: viewport3D.interactionMode === "SpaceMouse" ? "#FFFFFF" : "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("SpaceMouse selected")
                                    // Call C++ backend method to switch mode
                                    viewport3D.setInteractionMode("SpaceMouse")
                                }
                            }
                        }

                        // Trackpad Option (Multi-touch - Future Implementation)
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

                        // Mouse Option (Traditional 2D Input)
                        Rectangle {
                            width: parent.width
                            height: 32
                            // Dynamic color based on current interaction mode
                            color: viewport3D.interactionMode === "Mouse" ? "#4A90E2" : "#F8F9FA"
                            radius: 6
                            border.color: "#4A90E2"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "Mouse (Traditional)"
                                // Dynamic text color based on selection
                                color: viewport3D.interactionMode === "Mouse" ? "#FFFFFF" : "#2E3440"
                                font.pixelSize: 12
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("Mouse selected")
                                    // Call C++ backend method to switch mode
                                    viewport3D.setInteractionMode("Mouse")
                                }
                            }
                        }
                    }
                }

                // -----------------------------------------------------
                // 3D SHAPE SELECTION SECTION
                // -----------------------------------------------------
                // Choose between Cube, Sphere, Torus, Tetrahedron
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

                        // Cube Shape Option
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

                        // Sphere Shape Option
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

                        // Torus Shape Option
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

                        // Tetrahedron Shape Option (Default)
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

        // =================================================================
        // MAIN 3D VIEWPORT (50% width)
        // =================================================================
        // Central area containing the dual-model 3D renderer
        Rectangle {
            width: parent.width * 0.5
            height: parent.height
            color: "#F5F5F5"
            border.color: "#E1E5E9"
            border.width: 2
            radius: 8

            // -----------------------------------------------------
            // 3D OPENGL VIEWPORT COMPONENT
            // -----------------------------------------------------
            // Main 3D rendering area with dual model display
            OpenGL3DViewport {
                id: viewport3D
                anchors.fill: parent
                anchors.margins: 4

                // Initial 3D viewing configuration
                currentShape: 4  // Start with Tetrahedron
                translation: Qt.vector3d(0.0, 0.0, 0.0)
                rotation: Qt.vector3d(15.0, 25.0, 0.0)
                scale: 1.0

                // Research display settings
                showReferenceModel: true   // Show semi-transparent reference
                showMovableModel: true     // Show user-controlled model
                showVertexLabels: true     // Show alignment markers

                // Connect transform changes to update right panel displays
                onTransformChanged: {
                    translationDisplay.updateValues()
                    rotationDisplay.updateValues()
                    scaleDisplay.updateValues()
                }
            }

            // -----------------------------------------------------
            // TASK INSTRUCTIONS OVERLAY
            // -----------------------------------------------------
            // Floating instructions panel (top-left)
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 16
                width: 300
                height: taskInstructions.visible ? 140 : 0
                color: "#000000"
                opacity: 0.85
                radius: 8
                visible: true

                Column {
                    id: taskInstructions
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 6
                    visible: parent.visible

                    Text {
                        text: "🎯 Alignment Task"
                        color: "#FFFFFF"
                        font.pixelSize: 14
                        font.bold: true
                        font.family: "Arial"
                    }

                    Text {
                        text: "Objective: Align numbered corners"
                        color: "#4A90E2"
                        font.pixelSize: 12
                        font.family: "Arial"
                    }

                    Text {
                        text: "• White spheres = Reference (1', 2', 3'...)"
                        color: "#CCCCCC"
                        font.pixelSize: 10
                        font.family: "Arial"
                    }

                    Text {
                        text: "• Colored spheres = Movable (1, 2, 3...)"
                        color: "#CCCCCC"
                        font.pixelSize: 10
                        font.family: "Arial"
                    }

                    Text {
                        text: "• Match: 1→1', 2→2', 3→3', etc."
                        color: "#CCCCCC"
                        font.pixelSize: 10
                        font.family: "Arial"
                    }

                    Text {
                        text: "Target: < 0.300 units accuracy"
                        color: "#4CAF50"
                        font.pixelSize: 10
                        font.bold: true
                        font.family: "Arial"
                    }
                }

                // Close Instructions Button
                Rectangle {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 8
                    width: 20
                    height: 20
                    color: "#FF5722"
                    radius: 10

                    Text {
                        anchors.centerIn: parent
                        text: "×"
                        color: "#FFFFFF"
                        font.pixelSize: 12
                        font.bold: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: parent.parent.visible = false
                    }
                }
            }

            // -----------------------------------------------------
            // TASK COMPLETION MESSAGE
            // -----------------------------------------------------
            // Modal dialog shown when alignment task is completed
            Rectangle {
                id: taskCompletionMessage
                anchors.centerIn: parent
                width: 400
                height: 180
                color: "#2E3440"
                opacity: 0.95
                radius: 12
                visible: false
                z: 100  // Ensure it appears on top

                Column {
                    anchors.centerIn: parent
                    spacing: 15

                    Text {
                        text: "✅ Task Completed!"
                        color: "#4CAF50"
                        font.pixelSize: 22
                        font.bold: true
                        font.family: "Arial"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        id: taskSummaryText
                        text: "You have completed the alignment task.\nGet ready for the next shape."
                        color: "#FFFFFF"
                        font.pixelSize: 14
                        font.family: "Arial"
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    // Action Buttons Row
                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15

                        // Next Shape Button
                        Rectangle {
                            width: 100
                            height: 40
                            color: "#4A90E2"
                            radius: 8

                            Text {
                                anchors.centerIn: parent
                                text: "Next Shape"
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.bold: true
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    taskCompletionMessage.visible = false
                                    // Cycle to next shape
                                    var nextShape = (viewport3D.currentShape % 4) + 1
                                    viewport3D.currentShape = nextShape
                                    viewport3D.startAlignmentTask()
                                }
                            }
                        }

                        // Close Button
                        Rectangle {
                            width: 100
                            height: 40
                            color: "#607D8B"
                            radius: 8

                            Text {
                                anchors.centerIn: parent
                                text: "Close"
                                color: "#FFFFFF"
                                font.pixelSize: 12
                                font.bold: true
                                font.family: "Arial"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    taskCompletionMessage.visible = false
                                }
                            }
                        }
                    }
                }
            }

            // -----------------------------------------------------
            // REAL-TIME PERFORMANCE METRICS
            // -----------------------------------------------------
            // Bottom-right overlay showing current alignment accuracy
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 16
                width: 200
                height: 80
                color: "#000000"
                opacity: 0.7
                radius: 6

                Column {
                    anchors.centerIn: parent
                    spacing: 4

                    Text {
                        text: "Real-time Metrics"
                        color: "#FFFFFF"
                        font.pixelSize: 11
                        font.bold: true
                        font.family: "Arial"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: "Accuracy: " + viewport3D.alignmentAccuracy.toFixed(3)
                        color: {
                            if (viewport3D.alignmentAccuracy < 0.15) return "#4CAF50"      // Excellent - Green
                            else if (viewport3D.alignmentAccuracy < 0.3) return "#FF9800"  // Good - Orange
                            else return "#F44336"                                          // Needs work - Red
                        }
                        font.pixelSize: 10
                        font.family: "Consolas, Monaco, monospace"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: "Mode: " + viewport3D.interactionMode
                        color: "#4A90E2"
                        font.pixelSize: 10
                        font.family: "Arial"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: viewport3D.mousePressed ? "Transforming..." : "Ready"
                        color: viewport3D.mousePressed ? "#FF9800" : "#CCCCCC"
                        font.pixelSize: 10
                        font.family: "Arial"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            // -----------------------------------------------------
            // VIEWPORT LABEL
            // -----------------------------------------------------
            // Top-left label identifying the 3D viewport
            Text {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 12
                text: "Dual Model 3D Viewport"
                color: "#4A90E2"
                font.pixelSize: 14
                font.bold: true
                font.family: "Arial"
            }
        }

        // =================================================================
        // RIGHT CONTROL PANEL (25% width)
        // =================================================================
        // Contains: Task Controls, Display Options, Live Transform Data
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

                // -----------------------------------------------------
                // RESEARCH TASK CONTROLS SECTION
                // -----------------------------------------------------
                // Start/finish tasks, view current mode, accuracy display
                Rectangle {
                    width: parent.width
                    height: 220
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Research Task Control"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        // Current Interaction Mode Display
                        Column {
                            width: parent.width
                            spacing: 6

                            Text {
                                text: "Current Mode:"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#4A90E2"
                                font.family: "Arial"
                            }

                            Rectangle {
                                width: parent.width
                                height: 32
                                color: "#E8F4FD"
                                radius: 6
                                border.color: "#4A90E2"
                                border.width: 1

                                Text {
                                    anchors.centerIn: parent
                                    text: viewport3D.interactionMode + " Input"
                                    font.pixelSize: 11
                                    color: "#2E3440"
                                    font.family: "Arial"
                                }
                            }
                        }

                        // Task Control Buttons
                        Column {
                            width: parent.width
                            spacing: 8

                            // Start Task Button
                            Rectangle {
                                width: parent.width
                                height: 38
                                color: "#4CAF50"
                                radius: 6

                                Text {
                                    anchors.centerIn: parent
                                    text: "Start Task"
                                    font.pixelSize: 11
                                    font.bold: true
                                    color: "#FFFFFF"
                                    font.family: "Arial"
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("Research task started")
                                        viewport3D.startAlignmentTask()
                                    }
                                }
                            }

                            // Finish Task and Reset Buttons Row
                            Row {
                                width: parent.width
                                spacing: 8

                                // Finish Task Button (only enabled when task is active)
                                Rectangle {
                                    width: (parent.width - 8) / 2
                                    height: 38
                                    color: viewport3D.taskActive ? "#FF9800" : "#CCCCCC"
                                    radius: 6

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Finish Task"
                                        font.pixelSize: 11
                                        font.bold: true
                                        color: "#FFFFFF"
                                        font.family: "Arial"
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        enabled: viewport3D.taskActive
                                        onClicked: {
                                            console.log("Task finished manually")
                                            viewport3D.finishAlignmentTask()
                                            taskCompletionMessage.visible = true
                                        }
                                    }
                                }

                                // Reset Transform Button
                                Rectangle {
                                    width: (parent.width - 8) / 2
                                    height: 38
                                    color: "#F44336"
                                    radius: 6

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Reset"
                                        font.pixelSize: 11
                                        font.bold: true
                                        color: "#FFFFFF"
                                        font.family: "Arial"
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            console.log("Task reset")
                                            viewport3D.resetTransform()
                                        }
                                    }
                                }
                            }
                        }

                        // Current Alignment Accuracy Display
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
                                    text: "Alignment Accuracy"
                                    font.pixelSize: 10
                                    color: "#666666"
                                    font.family: "Arial"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }

                                Text {
                                    text: viewport3D.alignmentAccuracy.toFixed(3) + " units"
                                    font.pixelSize: 14
                                    font.bold: true
                                    color: {
                                        if (viewport3D.alignmentAccuracy < 0.15) return "#4CAF50"      // Excellent - Green
                                        else if (viewport3D.alignmentAccuracy < 0.3) return "#FF9800"  // Good - Orange
                                        else return "#F44336"                                          // Needs work - Red
                                    }
                                    font.family: "Consolas, Monaco, monospace"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                            }
                        }
                    }
                }

                // -----------------------------------------------------
                // DISPLAY OPTIONS SECTION
                // -----------------------------------------------------
                // Toggle visibility of reference model, movable model, vertex labels
                Rectangle {
                    width: parent.width
                    height: 200
                    color: "#FFFFFF"
                    border.color: "#E1E5E9"
                    border.width: 1
                    radius: 8

                    Column {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Display Options"
                            color: "#2E3440"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Arial"
                        }

                        // Reference Model Toggle (Semi-transparent reference)
                        Rectangle {
                            width: parent.width
                            height: 35
                            color: viewport3D.showReferenceModel ? "#4A90E2" : "#F8F9FA"
                            radius: 6
                            border.color: "#4A90E2"
                            border.width: 1

                            Row {
                                anchors.left: parent.left
                                anchors.leftMargin: 12
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8

                                // Checkbox indicator
                                Rectangle {
                                    width: 12
                                    height: 12
                                    radius: 6
                                    color: viewport3D.showReferenceModel ? "#FFFFFF" : "transparent"
                                    border.color: "#4A90E2"
                                    border.width: 1
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: "Show Reference Model (1', 2', 3'...)"
                                    color: viewport3D.showReferenceModel ? "#FFFFFF" : "#2E3440"
                                    font.pixelSize: 11
                                    font.family: "Arial"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: viewport3D.showReferenceModel = !viewport3D.showReferenceModel
                            }
                        }

                        // Movable Model Toggle (User-controlled model)
                        Rectangle {
                            width: parent.width
                            height: 35
                            color: viewport3D.showMovableModel ? "#4A90E2" : "#F8F9FA"
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
                                    color: viewport3D.showMovableModel ? "#FFFFFF" : "transparent"
                                    border.color: "#4A90E2"
                                    border.width: 1
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: "Show Movable Model (1, 2, 3...)"
                                    color: viewport3D.showMovableModel ? "#FFFFFF" : "#2E3440"
                                    font.pixelSize: 11
                                    font.family: "Arial"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: viewport3D.showMovableModel = !viewport3D.showMovableModel
                            }
                        }

                        // Vertex Labels Toggle (Alignment marker spheres)
                        Rectangle {
                            width: parent.width
                            height: 35
                            color: viewport3D.showVertexLabels ? "#4A90E2" : "#F8F9FA"
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
                                    color: viewport3D.showVertexLabels ? "#FFFFFF" : "transparent"
                                    border.color: "#4A90E2"
                                    border.width: 1
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: "Show Vertex Markers"
                                    color: viewport3D.showVertexLabels ? "#FFFFFF" : "#2E3440"
                                    font.pixelSize: 11
                                    font.family: "Arial"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: viewport3D.showVertexLabels = !viewport3D.showVertexLabels
                            }
                        }
                    }
                }

                // -----------------------------------------------------
                // LIVE TRANSFORM DISPLAY SECTION
                // -----------------------------------------------------
                // Real-time display of translation, rotation, and scale values
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

                        // Translation Display (X, Y, Z coordinates)
                        Column {
                            id: translationDisplay
                            width: parent.width
                            spacing: 6

                            // Function to update translation values
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

                        // Rotation Display (X, Y, Z angles in degrees)
                        Column {
                            id: rotationDisplay
                            width: parent.width
                            spacing: 6

                            // Function to update rotation values
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

                        // Scale Display (Uniform scaling factor)
                        Column {
                            id: scaleDisplay
                            width: parent.width
                            spacing: 6

                            // Function to update scale value
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

                // -----------------------------------------------------
                // DEVICE STATUS SECTION
                // -----------------------------------------------------
                // Shows current renderer status and task state
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
                            text: "Dual Model Renderer"
                            font.pixelSize: 12
                            font.bold: true
                            color: "#2E3440"
                            font.family: "Arial"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            text: viewport3D.taskActive ? "Task Active" : "Ready"
                            font.pixelSize: 10
                            color: viewport3D.taskActive ? "#4CAF50" : "#4A90E2"
                            font.family: "Arial"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
        }
    }

    // =================================================================
    // EVENT CONNECTIONS
    // =================================================================
    // Handle signals from the 3D viewport component

    // Research task completion handler
    Connections {
        target: viewport3D

        function onAlignmentCompleted(accuracy, timeMs) {
            console.log("Task completed! Accuracy:", accuracy, "Time:", timeMs, "ms")

            // Update completion message with task results
            taskSummaryText.text = "Task completed!\nAccuracy: " + accuracy.toFixed(3) + " units\nTime: " + (timeMs / 1000.0).toFixed(1) + " seconds\n\nReady for next shape?"

            // Show the completion dialog
            taskCompletionMessage.visible = true
        }
    }
}
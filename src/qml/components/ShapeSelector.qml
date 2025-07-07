import QtQuick 2.15

Rectangle {
    id: shapeSelector
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 6

    property string selectedShape: "tetrahedron"

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Text {
            text: "🔷 3D Shape"
            color: "#333333"
            font.pixelSize: 12
            font.bold: true
        }

        Column {
            width: parent.width
            spacing: 6

            // Cube Option
            Rectangle {
                width: parent.width
                height: 30
                color: shapeSelector.selectedShape === "cube" ? "#FF5722" : "#F8F9FA"
                radius: 4
                border.color: "#FF5722"
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
                        border.color: "#FF5722"
                        border.width: 1
                        color: shapeSelector.selectedShape === "cube" ? "#FF5722" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: shapeSelector.selectedShape === "cube"
                        }
                    }

                    Text {
                        text: "📦 Cube"
                        color: shapeSelector.selectedShape === "cube" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        shapeSelector.selectedShape = "cube"
                        console.log("Cube selected")
                    }
                }
            }

            // Sphere Option
            Rectangle {
                width: parent.width
                height: 30
                color: shapeSelector.selectedShape === "sphere" ? "#2196F3" : "#F8F9FA"
                radius: 4
                border.color: "#2196F3"
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
                        border.color: "#2196F3"
                        border.width: 1
                        color: shapeSelector.selectedShape === "sphere" ? "#2196F3" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: shapeSelector.selectedShape === "sphere"
                        }
                    }

                    Text {
                        text: "🌐 Sphere"
                        color: shapeSelector.selectedShape === "sphere" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        shapeSelector.selectedShape = "sphere"
                        console.log("Sphere selected")
                    }
                }
            }

            // Torus Option
            Rectangle {
                width: parent.width
                height: 30
                color: shapeSelector.selectedShape === "torus" ? "#4CAF50" : "#F8F9FA"
                radius: 4
                border.color: "#4CAF50"
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
                        border.color: "#4CAF50"
                        border.width: 1
                        color: shapeSelector.selectedShape === "torus" ? "#4CAF50" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: shapeSelector.selectedShape === "torus"
                        }
                    }

                    Text {
                        text: "🍩 Torus"
                        color: shapeSelector.selectedShape === "torus" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        shapeSelector.selectedShape = "torus"
                        console.log("Torus selected")
                    }
                }
            }

            // Tetrahedron Option
            Rectangle {
                width: parent.width
                height: 30
                color: shapeSelector.selectedShape === "tetrahedron" ? "#9C27B0" : "#F8F9FA"
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
                        color: shapeSelector.selectedShape === "tetrahedron" ? "#9C27B0" : "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            visible: shapeSelector.selectedShape === "tetrahedron"
                        }
                    }

                    Text {
                        text: "🔺 Tetrahedron"
                        color: shapeSelector.selectedShape === "tetrahedron" ? "#FFFFFF" : "#333333"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        shapeSelector.selectedShape = "tetrahedron"
                        console.log("Tetrahedron selected")
                    }
                }
            }
        }

        // Load Model Button
        Rectangle {
            width: parent.width
            height: 35
            color: "#607D8B"
            radius: 6
            border.color: "#455A64"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: "📁 Load Model..."
                color: "#FFFFFF"
                font.pixelSize: 11
                font.bold: true
            }

            MouseArea {
                anchors.fill: parent
                onClicked: console.log("Load model clicked")
            }
        }
    }
}

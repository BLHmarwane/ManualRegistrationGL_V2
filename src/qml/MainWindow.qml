import QtQuick 2.15
import QtQuick.Layouts 1.15
import "components"

Rectangle {
    id: mainWindow
    color: "#F8F9FA"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Main 3D Viewport (80% width)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.8
            color: "#FFFFFF"
            border.color: "#E8F4FF"
            border.width: 2

            Viewport3D {
                anchors.fill: parent
                anchors.margins: 4
            }
        }

        // Control Panel (20% width)
        Rectangle {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            color: "#FFFFFF"
            border.color: "#E8F4FF"
            border.width: 1

            Flickable {
                anchors.fill: parent
                anchors.margins: 24
                contentHeight: controlColumn.height

                Column {
                    id: controlColumn
                    width: parent.width
                    spacing: 16

                    // Header
                    Rectangle {
                        width: parent.width
                        height: 60
                        color: "#4A90E2"
                        radius: 8

                        Text {
                            anchors.centerIn: parent
                            text: "SURGAR Simulator V2"
                            color: "#FFFFFF"
                            font.pixelSize: 18
                            font.bold: true
                        }
                    }

                    // Mode Selection
                    ModeSelector {
                        width: parent.width
                        height: 140
                    }

                    // Shape Selection
                    ShapeSelector {
                        width: parent.width
                        height: 200
                    }

                    // Session Controls
                    SessionControls {
                        width: parent.width
                        height: 180
                    }

                    // Transform Display
                    TransformDisplay {
                        width: parent.width
                        height: 300
                    }
                }
            }
        }
    }
}
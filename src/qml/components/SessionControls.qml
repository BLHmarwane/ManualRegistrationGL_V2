import QtQuick 2.15

Rectangle {
    id: sessionControls
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 8
    
    property bool sessionActive: false
    
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16
        
        Text {
            text: "Session Control"
            color: "#333333"
            font.pixelSize: 18
            font.bold: true
        }
        
        Column {
            width: parent.width
            spacing: 12
            
            // Start Session Button
            Rectangle {
                width: parent.width
                height: 48
                color: !sessionControls.sessionActive ? "#4CAF50" : "#CCCCCC"
                radius: 8
                
                Text {
                    anchors.centerIn: parent
                    text: "Start Session"
                    font.pixelSize: 14
                    font.bold: true
                    color: "#FFFFFF"
                }
                
                MouseArea {
                    anchors.fill: parent
                    enabled: !sessionControls.sessionActive
                    onClicked: {
                        sessionControls.sessionActive = true
                        console.log("Session started")
                    }
                }
            }
            
            // Reset Transform Button
            Rectangle {
                width: parent.width
                height: 48
                color: sessionControls.sessionActive ? "#4A90E2" : "#CCCCCC"
                radius: 8
                
                Text {
                    anchors.centerIn: parent
                    text: "Reset Transform"
                    font.pixelSize: 14
                    color: "#FFFFFF"
                }
                
                MouseArea {
                    anchors.fill: parent
                    enabled: sessionControls.sessionActive
                    onClicked: console.log("Transform reset")
                }
            }
            
            // Finish Session Button
            Rectangle {
                width: parent.width
                height: 48
                color: sessionControls.sessionActive ? "#F44336" : "#CCCCCC"
                radius: 8
                
                Text {
                    anchors.centerIn: parent
                    text: "Finish Session"
                    font.pixelSize: 14
                    font.bold: true
                    color: "#FFFFFF"
                }
                
                MouseArea {
                    anchors.fill: parent
                    enabled: sessionControls.sessionActive
                    onClicked: {
                        sessionControls.sessionActive = false
                        console.log("Session finished")
                    }
                }
            }
        }
        
        // Session Status
        Rectangle {
            width: parent.width
            height: 30
            color: "#E8F4FF"
            radius: 8
            
            Text {
                anchors.centerIn: parent
                text: sessionControls.sessionActive ? "Session Active" : "Ready to Start"
                font.pixelSize: 12
                color: "#333333"
                font.bold: true
            }
        }
    }
}

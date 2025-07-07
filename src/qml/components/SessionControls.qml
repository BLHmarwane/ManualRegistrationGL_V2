import QtQuick 2.15

Rectangle {
    id: sessionControls
    color: "#FFFFFF"
    border.color: "#E8F4FF"
    border.width: 1
    radius: 6
    
    property bool sessionActive: false
    
    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8
        
        Text {
            text: "⚡ Session Control"
            color: "#333333"
            font.pixelSize: 12
            font.bold: true
        }
        
        Column {
            width: parent.width
            spacing: 6
            
            // Start Session Button
            Rectangle {
                width: parent.width
                height: 35
                color: !sessionControls.sessionActive ? "#4CAF50" : "#CCCCCC"
                radius: 6
                
                Text {
                    anchors.centerIn: parent
                    text: "▶️ Start Session"
                    font.pixelSize: 11
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
                height: 35
                color: sessionControls.sessionActive ? "#FF9800" : "#CCCCCC"
                radius: 6
                
                Text {
                    anchors.centerIn: parent
                    text: "🔄 Reset Transform"
                    font.pixelSize: 11
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
                height: 35
                color: sessionControls.sessionActive ? "#F44336" : "#CCCCCC"
                radius: 6
                
                Text {
                    anchors.centerIn: parent
                    text: "⏹️ Finish Session"
                    font.pixelSize: 11
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
            height: 25
            color: "#E8F4FF"
            radius: 4
            
            Text {
                anchors.centerIn: parent
                text: sessionControls.sessionActive ? "🟢 Active" : "⚪ Ready"
                font.pixelSize: 10
                color: "#333333"
                font.bold: true
            }
        }
    }
}

import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: root
    visible: true
    width: 1400
    height: 900
    minimumWidth: 1200
    minimumHeight: 800
    title: "Manual Registration Simulator V2 - SURGAR"
    color: "#F8F9FA"

    // Load main window content
    MainWindow {
        anchors.fill: parent
    }
}
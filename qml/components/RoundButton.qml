import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material 2.15

Button {
    id: button
    Material.background: "#3B82F6"
    Material.foreground: "white"
    opacity: enabled ? 1.0 : 0.5
    Behavior on opacity { NumberAnimation { duration: 200 } }
    font.pixelSize: 20
    font.family: "sans-serif"
    font.bold: true
    height: 80

    onClicked: root.clicked()
}
/*

Rectangle {

    property alias text: button.text
    property alias enabled: button.enabled
    property int buttonBottomMargin
    signal clicked()

    id: root
    color: "white"
    border.color: "#EEEEEE"
    
    Button {
        id: button
        Material.background: "#3B82F6"
        Material.foreground: "white"
        opacity: enabled ? 1.0 : 0.5
        Behavior on opacity { NumberAnimation { duration: 200 } }
        font.pixelSize: 20
        font.family: "sans-serif"
        font.bold: true
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 20
            bottomMargin: root.buttonBottomMargin
        }
        height: 76
        
        text: qsTr("Поехали!")
        onClicked: root.clicked()
    }
}*/

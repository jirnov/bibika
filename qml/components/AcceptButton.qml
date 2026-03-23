import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Button {
    id: button
    Material.background: "#3B82F6"
    Material.foreground: "white"
    opacity: enabled ? 1.0 : 0.5
    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }
    font.pixelSize: 20
    font.family: "sans-serif"
    font.bold: true
    height: 80
}

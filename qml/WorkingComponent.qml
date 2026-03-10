import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15 // Explicitly import for material properties
import QtQuick.Layouts

import BibikaService

ApplicationWindow {
    visible: true
    width: 400
    height: 400
    title: "Material ComboBox with Suggestions"

    Page {
        anchors.fill: parent

        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 20
            spacing: Style.defaultMargin

            TextField {
                id: inputField
                Material.accent: "black"
                Material.containerStyle: Material.Filled
                Layout.preferredHeight: 80
                Layout.fillWidth: true
                placeholderText: "Марка машины"

                background: Rectangle {
                    color: "white"
                    border.color: inputField.focus ? "black" : "lightgrey"
                    radius: 10
                }


                onTextEdited: {
                    suggestionPopup.open()
                }
            }

            Popup {
                id: suggestionPopup
                parent: inputField
                x: 0
                y: inputField.height
                width: inputField.width

                contentItem: ListView {
                    implicitHeight: contentHeight
                    model: {
                        let suggestions = ["SEAT", "BMW", "Лада", "Mercedes", "Cherry"]
                        var filtered = []
                        for (var i = 0; i < suggestions.length; ++i) {
                            let s = suggestions[i];
                            if (s.toLowerCase().startsWith(inputField.text.toLowerCase())) {
                                filtered.push(s);
                            }
                        }
                        return filtered
                    }

                    delegate: ItemDelegate {
                        width: suggestionPopup.width
                        text: modelData
                        onClicked: {
                            inputField.text = modelData
                            suggestionPopup.close()
                        }
                    }
                }

                background: Rectangle {
                    color: "white"
                    border.color: "lightgrey"
                    radius: 3
                }

            }

            Label {
                text: "Error"
                Layout.leftMargin: inputField.leftPadding
                font.pixelSize: 12
                color: "red"
            }
        }

    }
}

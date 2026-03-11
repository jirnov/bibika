pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 2.15

Item {
    id: root

    // Свойства интерфейса
    property alias placeholderText: inputField.placeholderText
    property alias text: inputField.text
    property string errorText: ""
    property bool digitsOnly: false
    property var suggestions: []

    // Сигналы
    signal editingFinished()
    signal updateSuggestions()
    signal validateField()

    // Размеры по умолчанию (берем от внутреннего layout)
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight

    // Внутренние функции
    function _getFiltererdSuggestions(searchText) {
        if (searchText.length === 0) {
            return []
        }

        var filtered = []
        for (var i = 0; i < suggestions.length; ++i) {
            if (suggestions[i].toLowerCase().startsWith(searchText.toLowerCase())) {
                filtered.push(suggestions[i])
            }
        }
        return filtered.sort()
    }

    // Основной layout
    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 1

        TextField {
            property var _digitsValidator: IntValidator {
                bottom: 0
            }

            property var _textValidator: null

            id: inputField
            Material.accent: "black"
            Material.containerStyle: Material.Filled
            Layout.preferredHeight: 56
            font.pixelSize: 16
            Layout.fillWidth: true

            inputMethodHints: root.digitsOnly ? Qt.ImhDigitsOnly : Qt.ImhNone
            validator: root.digitsOnly ? _digitsValidator : _textValidator

            background: Rectangle {
                color: "white"
                border.color: inputField.focus ? "grey" : "lightgrey"
                radius: 10
            }

            onEditingFinished: {
                root.validateField()
                root.editingFinished()
            }

            onTextChanged: {
                if (root.digitsOnly && text.length > 1 && text[0] === '0') {
                    text = text.replace(/^0+/, '')
                    if (text === '') {
                        text = '0';
                    }
                }
                root.validateField()
            }

            onTextEdited: {
                root.updateSuggestions()
                var filtered = root._getFiltererdSuggestions(inputField.text)
                suggestionsListView.model = filtered
                if (filtered.length > 0) {
                    root.editingFinished()
                    suggestionPopup.open()
                }
                else {
                    suggestionPopup.close()
                }
            }

            onActiveFocusChanged: {
                root.validateField()
            }
        }

        Label {
            text: root.errorText
            Layout.leftMargin: inputField.leftPadding
            font.pixelSize: 12
            opacity: root.errorText.length > 0 ? 1 : 0
            Behavior on opacity {
                NumberAnimation {
                    duration: 250
                }
            }
            color: "red"
        }
    }

    Popup {
        id: suggestionPopup
        parent: inputField
        y: inputField.height + 5
        width: inputField.width

        contentItem: ListView {
            id: suggestionsListView
            implicitHeight: contentHeight
            model: []

            delegate: ItemDelegate {
                required property string modelData

                leftPadding: 0
                rightPadding: 0
                Material.elevation: 0
                Material.background: "transparent"
                width: suggestionPopup.width - 24
                text: modelData

                onClicked: {
                    inputField.text = text
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

}

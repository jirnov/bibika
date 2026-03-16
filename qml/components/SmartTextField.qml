pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 2.15
import Qt5Compat.GraphicalEffects

import BibikaService

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

    function _trySuggestionOpen(searchText) {
        var filtered = root._getFiltererdSuggestions(searchText)
        suggestionsListView.model = filtered
        if (filtered.length > 0) {
            suggestionPopup.open()
        }
        else {
            suggestionPopup.close()
        }

    }

    function _getFiltererdSuggestions(searchText) {
        root.updateSuggestions()
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
        spacing: 0

        Item {
            Layout.preferredHeight: 80
            Layout.fillWidth: true

            DropShadow {
                anchors.fill: inputField
                source: inputField
                horizontalOffset: 0
                verticalOffset: 2
                radius: 16
                samples: 33
                color: "#1A000000"
                transparentBorder: true
            }

            TextField {
                anchors.fill: parent
                property var _digitsValidator: IntValidator {
                    bottom: 0
                }

                property var _textValidator: null

                id: inputField
                Material.accent: "black"
                Material.containerStyle: Material.Filled
                Layout.preferredHeight: 80
                font.pixelSize: Style.fontSizeDefault
                Layout.fillWidth: true

                inputMethodHints: root.digitsOnly ? Qt.ImhDigitsOnly : Qt.ImhNone
                validator: root.digitsOnly ? _digitsValidator : _textValidator

                background: Rectangle {
                    color: "white"
                    border.color: inputField.focus ? "#3B82F6" : "#E5E7EB"
                    radius: 12
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
                    if (_trySuggestionOpen(inputField.text)) {
                        root.editingFinished()
                    }
                }

                onActiveFocusChanged: {
                    root.validateField()

                    if (!focus) {
                        root.editingFinished()
                        suggestionPopup.close()
                    }
                    else {
                        _trySuggestionOpen(inputField.text)
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
        }

        Label {
            text: root.errorText
            Layout.leftMargin: inputField.leftPadding
            font.pixelSize: Style.fontSizeHint
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
        y: {
            if (AppSettings.keyboardHeight > 0) {
                var bottomY = inputField.height + 5 + suggestionPopup.height
                if (bottomY > AppSettings.keyboardHeight) {
                    return inputField.y - 5 - suggestionPopup.height
                }
            }
            return inputField.height + 5;

/*
            console.log("keyboard height: " + AppSettings.keyboardHeight)
            console.log("popup height: " + suggestionPopup.height);
            console.log("popup y: " + (inputField.height + 5))
            if (AppSettings.keyboardHeight > 0) {
                inputField.y - 5 - suggestionPopup.implicitContentHeight
            }
            return inputField.height + 5*/
        }
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

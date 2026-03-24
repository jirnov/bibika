pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import BibikaService

// TODO: Не показывать errorText при выборе варианта из Popup
Item {
    id: root

    // Свойства интерфейса
    property alias placeholderText: inputField.placeholderText
    property alias text: inputField.text
    property string errorText: ""
    property bool digitsOnly: false
    property var suggestions: []

    // Вызывается при изменении текста
    signal editingFinished
    // Вызывается для обновления suggestions
    signal updateSuggestions
    // Проверка поля
    signal validateField

    // Размеры по умолчанию (берем от внутреннего layout)
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight

    property Timer _validateTimer: Timer {
        interval: 500
        onTriggered: {
            root.validateField();
        }
    }

    function _trySuggestionOpen(searchText) {
        var filtered = root._getFiltererdSuggestions(searchText);
        suggestionsListView.model = filtered;
        if (filtered.length > 0 && inputField.activeFocus) {
            suggestionPopup.show();
        } else {
            suggestionPopup.close();
        }
    }

    function _getFiltererdSuggestions(searchText) {
        root.updateSuggestions();
        if (searchText.length === 0) {
            return [];
        }

        var filtered = [];
        for (var i = 0; i < suggestions.length; ++i) {
            if (suggestions[i].toLowerCase().startsWith(searchText.toLowerCase())) {
                filtered.push(suggestions[i]);
            }
        }
        return filtered.sort();
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
                id: inputField
                anchors.fill: parent
                property var _digitsValidator: IntValidator {
                    bottom: 0
                }

                property var _textValidator: null
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
                    root._validateTimer.restart();
                }

                onTextChanged: {
                    if (root.digitsOnly && text.length > 1 && text[0] === '0') {
                        text = text.replace(/^0+/, '');
                        if (text === '') {
                            text = '0';
                        }
                    }
                    root._validateTimer.restart();
                }

                onTextEdited: {
                    root._trySuggestionOpen(inputField.text);
                }

                onActiveFocusChanged: {
                    root.validateField();

                    if (!focus) {
                        root.editingFinished();
                        suggestionPopup.close();
                    } else {
                        root._trySuggestionOpen(inputField.text);
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
        parent: Overlay.overlay

        function show() {
            if (!inputField || !inputField.visible) {
                return;
            }

            var fieldPos = inputField.mapToItem(null, 0, 0);
            x = fieldPos.x;
            y = fieldPos.y + inputField.height + 5;

            if (AppSettings.keyboardHeight > 0) {
                var bottomY = inputField.height + 5 + suggestionPopup.height;
                if (bottomY > AppSettings.keyboardHeight) {
                    y = fieldPos.y - 5 - suggestionPopup.height;
                }
            }

            open();
        }
        width: inputField.width
        height: Math.min(contentItem.implicitHeight + 10, 300)

        contentItem: ListView {
            id: suggestionsListView
            implicitHeight: Math.min(contentHeight, 300)
            model: []
            clip: true

            delegate: ItemDelegate {
                required property string modelData

                leftPadding: 0
                rightPadding: 0
                Material.elevation: 0
                Material.background: "transparent"
                width: suggestionPopup.width - 24
                text: modelData

                onClicked: {
                    inputField.text = text;
                    suggestionPopup.close();
                    root.validateField();
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }

        background: Rectangle {
            color: "white"
            border.color: "lightgrey"
            radius: 3
        }
    }
}

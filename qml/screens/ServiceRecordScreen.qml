import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import BibikaService

Page {
    id: root
    width: parent ? parent.width : AppSettings.width
    height: parent ? parent.height : AppSettings.height

    required property int recordId

    property ServiceRecord _editCopy: null

    signal recordUpdated(int recordId, ServiceRecord record)
    signal recordCreated(ServiceRecord record)

    Material.accent: "black"
    Material.containerStyle: Material.Filled

    Component.onCompleted: {
        if (recordId !== 0) {
            _editCopy = ServiceRecordModel.getById(recordId, root);
            let msg = `Редактирование записи: ${_editCopy.name} на пробеге ${Qt.locale().toString(_editCopy.mileage)} километров, ${Qt.formatDateTime(_editCopy.serviceDate, "dd.MM.yyyy")}`;
            console.log(msg);
        } else {
            _editCopy = ServiceRecordBuilder.createEmpty(root);
            _editCopy.eventType = ServiceRecord.Maintenance;
            _editCopy.mileage = AppSettings.carInfo.mileage;
            _editCopy.serviceDate = new Date();
            console.log(`Создание новой записи с текущим пробегом: ${Qt.locale().toString(_editCopy.mileage)} на дату ${Qt.formatDateTime(_editCopy.serviceDate, "dd.MM.yyyy")}`);
        }
    }

    Component.onDestruction: {
        if (root._editCopy !== null) {
            root._editCopy.destroy();
        }
    }

    readonly property color accentColor: "#000000"
    readonly property color textColor: "#333333"
    readonly property color secondaryTextColor: "#666666"

    header: ToolBar {
        background: Rectangle {
            color: "white"
        }

        Label {
            text: "Редактирование события"
            anchors.centerIn: parent
            font.pixelSize: Style.fontSizeTitle
            font.bold: true
        }
    }

    footer: ToolBar {

        background: Rectangle {
            color: "transparent"
            border.color: "transparent"
        }

        RowLayout {
            anchors.fill: parent
            spacing: Style.defaultMargin

            AcceptButton {
                Layout.fillWidth: true
                Layout.margins: Style.horizontalMargin
                text: qsTr("Сохранить")

                onClicked: {
                    // TODO: Добавить валидацию данных
                    if (root.recordId === 0) {
                        root.recordCreated(root._editCopy);
                    } else {
                        root.recordUpdated(root.recordId, root._editCopy);
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Style.backgroundColor

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Style.defaultMargin
                Layout.margins: Style.defaultMargin
                Layout.bottomMargin: Style.defaultMargin

                // Тип события
                TabBar {
                    id: eventType
                    Layout.fillWidth: true
                    currentIndex: Number(root._editCopy.eventType) || 0

                    background: Rectangle {
                        color: "transparent"
                    }

                    TabButton {
                        id: repairTabButton
                        text: "🔧 Ремонт"
                        font.pixelSize: 12
                        contentItem: Text {
                            text: repairTabButton.text
                            font: repairTabButton.font
                            color: eventType.currentIndex === 0 ? root.accentColor : root.secondaryTextColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: "transparent"
                            Rectangle {
                                width: parent.width
                                height: 2
                                color: eventType.currentIndex === 0 ? root.accentColor : "transparent"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }

                    TabButton {
                        id: maintenanceTabButton
                        text: "📅 ТО"
                        font.pixelSize: 12
                        contentItem: Text {
                            text: maintenanceTabButton.text
                            font: maintenanceTabButton.font
                            color: eventType.currentIndex === 1 ? root.accentColor : root.secondaryTextColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: "transparent"
                            Rectangle {
                                width: parent.width
                                height: 2
                                color: eventType.currentIndex === 1 ? root.accentColor : "transparent"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }

                    TabButton {
                        id: serviceTabButton
                        text: "🧼 Услуга"
                        font.pixelSize: 12
                        contentItem: Text {
                            text: serviceTabButton.text
                            font: serviceTabButton.font
                            color: eventType.currentIndex === 2 ? root.accentColor : root.secondaryTextColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: "transparent"
                            Rectangle {
                                width: parent.width
                                height: 2
                                color: eventType.currentIndex === 2 ? root.accentColor : "transparent"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }

                    onCurrentIndexChanged: {
                        console.log("new event type: ", currentIndex);
                        if (root._editCopy) {
                            root._editCopy.eventType = currentIndex;
                        }
                    }
                }
            }

            // СРЕДНЯЯ ЧАСТЬ (прокручиваемая, занимает всё оставшееся место)
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: Style.defaultMargin
                Layout.rightMargin: Style.defaultMargin
                clip: true

                ScrollBar.vertical: ScrollBar {
                    width: 4
                    policy: ScrollBar.AsNeeded
                }

                ColumnLayout {
                    width: parent.width
                    spacing: Style.defaultMargin

                    // Название события
                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: children[0].implicitHeight + 24
                        color: Style.cardColor
                        radius: 8

                        ColumnLayout {
                            id: nameColumn
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: Style.defaultMargin

                            Label {
                                text: "Название:"
                                font.pixelSize: 12
                                color: root.secondaryTextColor
                            }

                            TextField {
                                id: nameInput
                                Layout.preferredHeight: 30
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                text: root._editCopy.name
                                onTextEdited: root._editCopy.name = text
                            }
                            Label {
                                id: nameHint
                                Layout.fillWidth: true
                                color: "#ff5252"
                                font.pixelSize: 10
                                //visible: text !== ""
                                text: "Обязательное поле"
                            }
                        }
                    }

                    // Стоимость
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: children[0].implicitHeight + 24
                        color: Style.cardColor
                        radius: 8

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: Style.defaultMargin

                            Label {
                                text: "Стоимость (₽):"
                                font.pixelSize: 12
                                color: root.secondaryTextColor
                            }

                            TextField {
                                Layout.preferredHeight: 30
                                Layout.fillWidth: true
                                text: String(root._editCopy.price)
                                validator: IntValidator {
                                    bottom: 0
                                }
                                font.pixelSize: 14
                                onTextEdited: root._editCopy.price = Number(text) || 0
                            }
                        }
                    }

                    // Пробег и дата
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: children[0].implicitHeight + 24
                        color: Style.cardColor
                        radius: 8

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: Style.defaultMargin

                            RowLayout {
                                Label {
                                    text: "Пробег (км):"
                                    font.pixelSize: 12
                                    color: root.secondaryTextColor
                                }

                                TextField {
                                    id: mileageField
                                    Layout.preferredHeight: 30
                                    Layout.fillWidth: true
                                    text: String(root._editCopy.mileage)
                                    font.pixelSize: 14
                                    validator: IntValidator {
                                        bottom: 0
                                    }
                                    onTextEdited: root._editCopy.mileage = Number(text) || 0
                                }
                            }

                            RowLayout {
                                Label {
                                    text: "Дата события:"
                                    font.pixelSize: 12
                                    color: root.secondaryTextColor
                                }

                                TextField {
                                    Layout.preferredHeight: 30
                                    Layout.fillWidth: true
                                    text: Qt.formatDateTime(root._editCopy.serviceDate, "dd.MM.yyyy")
                                    inputMethodHints: Qt.ImhDate
                                    font.pixelSize: 14
                                    inputMask: "00\\.00\\.0000;_"

                                    validator: RegularExpressionValidator {
                                        regularExpression: /\d{2}.\d{2}\.\d{4}/
                                    }

                                    onEditingFinished: {
                                        var parts = text.split('.');
                                        if (parts.length === 3) {
                                            var day = parseInt(parts[0]);
                                            var month = parseInt(parts[1]) - 1; // Месяцы с 0
                                            var year = parseInt(parts[2]);

                                            var newDate = new Date(year, month, day);

                                            // Проверяем валидность даты
                                            if (!isNaN(newDate.getDate())) {
                                                root._editCopy.serviceDate = newDate;
                                            } else {
                                                // Если дата невалидна, возвращаем старое значение
                                                text = Qt.formatDateTime(root._editCopy.serviceDate, "dd.MM.yyyy");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Заметка
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 70
                        color: Style.cardColor
                        radius: 8

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: Style.defaultMargin

                            Label {
                                text: "Заметка:"
                                font.pixelSize: 10
                                color: root.secondaryTextColor
                            }

                            TextField {
                                Layout.preferredHeight: 30
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                text: root._editCopy.notes
                                onTextEdited: root._editCopy.notes = text
                            }
                        }
                    }

                    // Повторить через ТО (условно видимый блок)
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: children[0].implicitHeight + 24
                        color: Style.cardColor
                        visible: eventType.currentIndex === 1
                        radius: 8

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: Style.defaultMargin

                            Label {
                                text: "Повторить через"
                                font.pixelSize: 12
                                color: root.secondaryTextColor
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                CheckBox {
                                    id: useRepeatAfterDistance
                                    Layout.preferredWidth: 20
                                    checked: root._editCopy.hasRepeatAfterDistance
                                    onCheckedChanged: root._editCopy.hasRepeatAfterDistance = checked
                                }
                                TextField {
                                    id: repeatDistanceInput
                                    text: String(root._editCopy.repeatAfterDistance)
                                    enabled: useRepeatAfterDistance.checked
                                    horizontalAlignment: TextInput.AlignRight
                                    validator: IntValidator {
                                        bottom: 0
                                    }
                                    implicitWidth: 80
                                    font.pixelSize: 14
                                    onTextEdited: root._editCopy.repeatAfterDistance = Number(text) || 0
                                }
                                Label {
                                    enabled: useRepeatAfterDistance.checked
                                    text: qsTr("километров")
                                    font.pixelSize: 14
                                }
                                Item {
                                    Layout.fillWidth: true
                                }
                            }
                            Label {
                                text: qsTr("или (что наступит раньше)")
                                font.pixelSize: 12
                                color: root.secondaryTextColor
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                CheckBox {
                                    id: useRepeatAfterMonths
                                    Layout.preferredWidth: 20
                                    checked: root._editCopy.hasRepeatAfterMonths
                                    onCheckedChanged: root._editCopy.hasRepeatAfterMonths = checked
                                }
                                TextField {
                                    id: repeatMonthsInput
                                    text: String(root._editCopy.repeatAfterMonths)
                                    implicitWidth: 40
                                    horizontalAlignment: TextInput.AlignRight
                                    font.pixelSize: 14
                                    enabled: useRepeatAfterMonths.checked
                                    validator: IntValidator {
                                        bottom: 0
                                    }
                                    onTextEdited: root._editCopy.repeatAfterMonths = Number(text) || 0
                                }
                                Label {
                                    text: qsTr("месяцев")
                                    font.pixelSize: 14
                                    enabled: useRepeatAfterMonths.checked
                                }
                                Item {
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    // Дополнительный отступ внизу для красоты
                    Item {
                        Layout.preferredHeight: Style.defaultMargin
                    }
                }
            }
        }
    }
}

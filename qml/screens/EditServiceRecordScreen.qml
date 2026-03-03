import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: editServiceRecordScreen
    width: 288
    height: 640
    visible: true

    property int currentMileage: 232000
    property date currentDate: new Date()

    readonly property color accentColor: "#4CAF50"
    readonly property color backgroundColor: "#f0f0f0"
    readonly property color cardColor: "#FFFFFF"
    readonly property color textColor: "#333333"
    readonly property color secondaryTextColor: "#666666"

    Rectangle {
        anchors.fill: parent
        color: backgroundColor

        // Главный вертикальный layout на весь экран
        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // ВЕРХНЯЯ ЧАСТЬ (фиксированная, прижата к верху)
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Layout.margins: 8
                Layout.bottomMargin: 8

                // Заголовок
                Label {
                    text: "Редактирование события"
                    font.pixelSize: 20
                    font.bold: true
                    color: textColor
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 4
                }

                // Тип события (TabBar)
                TabBar {
                    id: eventType
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    currentIndex: 0

                    background: Rectangle {
                        color: "transparent"
                    }

                    TabButton {
                        text: "🔧 Ремонт"
                        font.pixelSize: 12
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: eventType.currentIndex === 0 ? accentColor : secondaryTextColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: "transparent"
                            Rectangle {
                                width: parent.width
                                height: 2
                                color: eventType.currentIndex === 0 ? accentColor : "transparent"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }

                    TabButton {
                        text: "📅 ТО"
                        font.pixelSize: 12
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: eventType.currentIndex === 1 ? accentColor : secondaryTextColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: "transparent"
                            Rectangle {
                                width: parent.width
                                height: 2
                                color: eventType.currentIndex === 1 ? accentColor : "transparent"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }

                    TabButton {
                        text: "🧼 Услуга"
                        font.pixelSize: 12
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: eventType.currentIndex === 2 ? accentColor : secondaryTextColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: "transparent"
                            Rectangle {
                                width: parent.width
                                height: 2
                                color: eventType.currentIndex === 2 ? accentColor : "transparent"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }
                }

                // Статус события (тоже в верхней части)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: children[0].implicitHeight + 24
                    //color: cardColor

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        Label {
                            text: "Статус:"
                            font.pixelSize: 12
                            color: secondaryTextColor
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            CheckBox {
                                id: eventStatus
                                checked: true

                                indicator: null

                                contentItem: Text {
                                    text: eventStatus.checked ? "✅ <b>Выполнено</b> / Запланировано" : "🔔 Выполнено / <b>Запланировано</b>"
                                    textFormat: Text.RichText
                                    font.pixelSize: 14
                                    color: textColor
                                }
                            }
                        }
                    }
                }
            }

            // СРЕДНЯЯ ЧАСТЬ (прокручиваемая, занимает всё оставшееся место)
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                clip: true

                ScrollBar.vertical: ScrollBar {
                    width: 4
                    policy: ScrollBar.AsNeeded
                }

                ColumnLayout {
                    width: parent.width
                    spacing: 8

                    // Название события
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 70
                        color: cardColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            Label {
                                text: "Название:"
                                font.pixelSize: 12
                                color: secondaryTextColor
                            }

                            TextField {
                                Layout.preferredHeight: 30
                                Layout.fillWidth: true
                                font.pixelSize: 14
                            }
                        }
                    }

                    // Стоимость
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: children[0].implicitHeight + 24
                        color: cardColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            Label {
                                text: "Стоимость (₽):"
                                font.pixelSize: 12
                                color: secondaryTextColor
                            }

                            TextField {
                                Layout.preferredHeight: 30
                                Layout.fillWidth: true
                                text: "0"
                                validator: IntValidator {
                                    bottom: 0
                                }
                                font.pixelSize: 14
                            }
                        }
                    }

                    // Пробег и дата
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: children[0].implicitHeight + 24
                        color: cardColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            RowLayout {

                                Label {
                                    text: "Пробег (км):"
                                    font.pixelSize: 12
                                    color: secondaryTextColor
                                }

                                TextField {
                                    Layout.preferredHeight: 30
                                    Layout.fillWidth: true
                                    text: String(currentMileage)
                                    font.pixelSize: 14
                                }
                            }

                            RowLayout {

                                Label {
                                    text: "Дата события:"
                                    font.pixelSize: 12
                                    color: secondaryTextColor
                                }

                                TextField {
                                    Layout.preferredHeight: 30
                                    Layout.fillWidth: true
                                    text: Qt.formatDateTime(currentDate, "dd.MM.yyyy")
                                    inputMethodHints: Qt.ImhDate
                                    font.pixelSize: 14
                                    inputMask: "00\\.00\\.0000;_"

                                    validator: RegularExpressionValidator {
                                        regularExpression: /\d{2}.\d{2}\.\d{4}/
                                    }
                                }
                            }
                        }
                    }

                    // Комментарий
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 70
                        color: cardColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            Label {
                                text: "Комментарий:"
                                font.pixelSize: 12
                                color: secondaryTextColor
                            }

                            TextField {
                                Layout.preferredHeight: 30
                                Layout.fillWidth: true
                                font.pixelSize: 14
                            }
                        }
                    }

                    // Повторить ТО (условно видимый блок)
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: children[0].implicitHeight + 24
                        color: cardColor
                        visible: eventStatus.checked && eventType.currentIndex === 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            Label {
                                text: "Следующее ТО:"
                                font.pixelSize: 12
                                color: secondaryTextColor
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                CheckBox {
                                    id: intervalMileageBox
                                    Layout.preferredWidth: 20
                                }
                                Label {
                                    text: "каждые"
                                    enabled: intervalMileageBox.checked
                                }
                                TextField {
                                    text: "15000"
                                    enabled: intervalMileageBox.checked
                                    horizontalAlignment: TextInput.AlignRight
                                    implicitWidth: 60
                                    font.pixelSize: 14
                                }
                                Label {
                                    enabled: intervalMileageBox.checked
                                    text: "км"
                                    font.pixelSize: 14
                                }
                                Item { Layout.fillWidth: true }
                            }
                            Label {
                                text: "или (что наступит раньше)"
                                font.pixelSize: 12
                                color: secondaryTextColor
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                CheckBox {
                                    id: intervalTimeBox
                                    Layout.preferredWidth: 20
                                }
                                Label {
                                    text: "каждые"
                                    enabled: intervalTimeBox.checked
                                }
                                TextField {
                                    text: "12"
                                    implicitWidth: 40
                                    horizontalAlignment: TextInput.AlignRight
                                    font.pixelSize: 14
                                    enabled: intervalTimeBox.checked
                                }
                                Label {
                                    text: "месяцев"
                                    font.pixelSize: 14
                                    enabled: intervalTimeBox.checked
                                }
                                Item { Layout.fillWidth: true }
                            }
                        }
                    }

                    // Дополнительный отступ внизу для красоты
                    Item {
                        Layout.preferredHeight: 8
                    }
                }
            }

            // НИЖНЯЯ ЧАСТЬ (фиксированная, прижата к низу)
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 8
                spacing: 8

                Button {
                    Layout.fillWidth: true
                    text: eventStatus.checked ? "Добавить" : "Запланировать"
                }

                Button {
                    Layout.fillWidth: true
                    text: "Закрыть"
                }
            }
        }
    }
}

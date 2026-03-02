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
    readonly property color backgroundColor: "#F5F5F5"
    readonly property color cardColor: "#FFFFFF"
    readonly property color textColor: "#333333"
    readonly property color secondaryTextColor: "#666666"

    Rectangle {
        anchors.fill: parent
        color: backgroundColor

        ScrollView {
            anchors.fill: parent
            clip: true
            contentWidth: parent.width

            ScrollBar.vertical: ScrollBar {
                width: 4
                policy: ScrollBar.AsNeeded
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8

                // Заголовок
                Label {
                    text: "Редактирование события"
                    font.pixelSize: 20
                    font.bold: true
                    color: textColor
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 4
                }

                // Тип события
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

                // Статус события
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: children[0].implicitHeight + 24
                    color: cardColor
                    radius: 12

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        Label {
                            text: "Статус события:"
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
                                    text: eventStatus.checked ? "✅ Выполнено" : "🔔 Запланировано"
                                    font.pixelSize: 14
                                    font.bold: eventStatus.checked
                                    color: textColor
                                    leftPadding: eventStatus.indicator.width + eventStatus.spacing
                                }
                            }
                        }
                    }
                }

                // Название события
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 70
                    color: cardColor
                    radius: 12

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
                            //placeholderText: "Замена масла, фильтров..."
                            font.pixelSize: 14
                            /*
                            background: Rectangle {
                                color: "transparent"
                                implicitHeight: 30
                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: secondaryTextColor
                                    anchors.bottom: parent.bottom
                                }
                            }*/
                        }
                    }
                }

                // Стоимость
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: children[0].implicitHeight + 24
                    color: cardColor
                    radius: 12

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        Label {
                            text: "Стоимость:"
                        }

                        TextField {
                            Layout.preferredHeight: 30
                            Layout.fillWidth: true
                            placeholderText: ""
                            validator: IntValidator {
                                bottom: 0
                            }

                            font.pixelSize: 14
                            /*
                            background: Rectangle {
                                color: "transparent"
                                implicitHeight: 30
                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: secondaryTextColor
                                    anchors.bottom: parent.bottom
                                }
                            }*/

                        }
                    }
                }

                // Выполнено на пробеге
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: children[0].implicitHeight + 24
                    color: cardColor
                    radius: 12

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

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
                            /*
                            background: Rectangle {
                                color: "transparent"
                                implicitHeight: 30
                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: secondaryTextColor
                                    anchors.bottom: parent.bottom
                                }
                            }*/
                        }

                        Label {
                            text: "Дата события: "
                            font.pixelSize: 12
                            color: secondaryTextColor
                        }

                        TextField {
                            Layout.preferredHeight: 30
                            Layout.fillWidth: true
                            text: Qt.formatDateTime(currentDate, "dd.MM.yyyy")
                            inputMethodHints: Qt.ImhDate
                            font.pixelSize: 14
                            cursorVisible: true

                            inputMask: "00\\.00\\.0000;_"

                            validator: RegularExpressionValidator {
                                regularExpression: /\d{2}.\d{2}\.\d{4}/
                            }
                            /*
                            background: Rectangle {
                                color: "transparent"
                                implicitHeight: 30
                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: secondaryTextColor
                                    anchors.bottom: parent.bottom
                                }
                            }
                            */
                        }
                    }
                }

                // Комментарий
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    color: cardColor
                    radius: 12

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        Label {
                            text: "Комментарий:"
                        }

                        TextArea {
                            Layout.fillWidth: true
                            Layout.fillHeight: true  // Занимает всю доступную высоту
                            //placeholderText: "Комментарий"
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignTop
                            wrapMode: TextArea.Wrap  // Перенос текста

                            // Устанавливаем минимальную высоту для 3 строк
                            implicitHeight: 70  // Примерно 3 строки текста
                            font.pixelSize: 14
                            /*
                            background: Rectangle {
                                color: "transparent"
                                implicitHeight: 30
                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: secondaryTextColor
                                    anchors.bottom: parent.bottom
                                }
                            }
*/
                        }
                    }
                }

                // Повторить ТО
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: children[0].implicitHeight + 24
                    color: cardColor
                    radius: 12
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
                            spacing: 4

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
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 4

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
                                font.pixelSize: 14
                                text: "месяцев"
                                enabled: intervalTimeBox.checked
                            }
                            Item { Layout.fillWidth: true }
                        }
                    }
                }

                // Кнопки
                RowLayout {
                    Layout.fillWidth: true
                    Layout.topMargin: 8
                    Layout.bottomMargin: 8
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

                Item {
                    Layout.preferredHeight: 10
                }
            }
        }
    }
}

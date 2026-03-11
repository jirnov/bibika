import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: AppSettings.width
    height: AppSettings.height

    property string _carName: AppSettings.carInfo.name
    property int _mileage: AppSettings.carInfo.lastMileage
    property date _mileageDate: AppSettings.carInfo.lastMileageDate

    signal openAddRecordDialog()
    signal openEditRecordDialog(int index)
    signal openSettingsDialog()
    signal openMileageDialog()
/*
    header: ToolBar{
        ColumnLayout {
            anchors.fill: parent
            Label {
                text: "BibikaService"
                font.pixelSize: Style.fontSizeTitle
                font.bold: true
                color: Style.titleColor
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
*/

    Flickable {
        anchors.fill: parent

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            spacing: 25

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                color: "white"
                border.color: "#eeeeee"

                Text {
                    anchors.centerIn: parent
                    text: qsTr("Главный экран")
                    font.pixelSize: Style.fontSizeTitle
                    font.bold: true
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Text {
                    text: qsTr("Ваш автомобиль: " + root._carName)
                    font.pixelSize: Style.fontSizeDefault
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Text {
                    text: qsTr("Текущий пробег:" + root._mileage)
                    font.pixelSize: Style.fontSizeDefault
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Text {
                    text: qsTr("пробег обновлён:" + Qt.formatDate(root._mileageDate, "dd.MM.yyyy"))
                    font.pixelSize: Style.fontSizeDefault
                }
            }
        }

    }

/*
    Rectangle {
        color: Style.cardColor
        radius: 8

        ColumnLayout {
            Label {
                text: "Ваш автомобиль: " + root._carName
            }
            Label {
                text: "Текущий пробег (км): " + root._mileage
            }
            Label {
                text: "Последнее обновление: " + Qt.formatDate(root._mileageDate, "dd.MM.yyyy")
            }
            Item {
                Layout.fillHeight: true
            }

            Button {
                text: "Добавить записи"
                onClicked: root.openAddRecordDialog()
            }
            Button {
                text: "Открыть настройки"
                onClicked: root.openSettingsDialog()
            }
            Button {
                text: "Обновить пробег"
                onClicked: root.openMileageDialog()
            }
            Button {
                text: "Обновить запись"
                onClicked: root.openEditRecordDialog(100500)
            }
        }
    }

    /*

    ListModel {
        id: upcomingModel
        ListElement {
            name: "Замена масла"
            mileage: 20000
            current: 123456
            remaining: 500
            cost: 5500
            type: "regular"
        }
        ListElement {
            name: "Замена фильтров"
            mileage: 20000
            current: 123456
            remaining: 500
            cost: 2500
            type: "regular"
        }
        ListElement {
            name: "Замена свечей"
            mileage: 30000
            current: 123456
            remaining: 6544
            cost: 4500
            type: "regular"
        }
        ListElement {
            name: "Тормозные колодки"
            mileage: 15000
            current: 123456
            remaining: 2500
            cost: 8200
            type: "regular"
        }
    }

    ListModel {
        id: historyModel
        ListElement {
            name: "Замена масла"
            date: "24.02.2026"
            mileage: 123456
            cost: 7100
        }
        ListElement {
            name: "Колодки тормозные"
            date: "15.01.2026"
            mileage: 118500
            cost: 8200
        }
        ListElement {
            name: "Антифриз"
            date: "01.12.2025"
            mileage: 112300
            cost: 3400
        }
    }

    // Основной контейнер с прокруткой
    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: mainColumn.height
        clip: true

        ColumnLayout {
            id: mainColumn
            width: parent.width
            spacing: 0

            // Баннер напоминания (появляется если прошло >7 дней)
            Rectangle {
                visible: false
                Layout.fillWidth: true
                height: 56
                color: colorWarning

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16

                    Label {
                        text: "⏰"
                        font.pixelSize: 20
                    }

                    Label {
                        text: "Обновите пробег"
                        color: "white"
                        font.weight: Font.Medium
                        Layout.fillWidth: true
                    }

                    Button {
                        text: "Ввести"
                        flat: true
                        Material.foreground: "white"
                        onClicked: mileageDialog.open()
                    }
                }
            }

            // Карточка текущего пробега
            Rectangle {
                Layout.fillWidth: true
                Layout.margins: 16
                height: 120
                color: "white"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16

                    Label {
                        text: "ТЕКУЩИЙ ПРОБЕГ"
                        font.pixelSize: 12
                        color: textSecondary
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: currentMileage.toLocaleString()
                            font.pixelSize: 36
                            font.weight: Font.Bold
                            color: textPrimary
                        }

                        Label {
                            text: "км"
                            font.pixelSize: 16
                            color: textSecondary
                            Layout.alignment: Qt.AlignBottom
                            bottomPadding: 4
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Button {
                            text: "Обновить"
                            highlighted: true
                            Material.background: colorPrimary
                            onClicked: mileageDialog.open()
                        }
                    }

                    Label {
                        text: "Обновлено: " + lastUpdated
                        font.pixelSize: 12
                        color: textSecondary
                    }
                }
            }

            // Карточка расходов
            Rectangle {
                Layout.fillWidth: true
                Layout.margins: 16
                Layout.topMargin: 0
                height: 80
                color: colorAccent
                radius: 8

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16

                    Label {
                        text: "💰"
                        font.pixelSize: 24
                    }

                    ColumnLayout {
                        Label {
                            text: "РАСХОДЫ ЗА ВСЁ ВРЕМЯ"
                            font.pixelSize: 12
                            color: textPrimary
                            opacity: 0.7
                        }

                        Label {
                            text: totalExpenses.toLocaleString() + " ₽"
                            font.pixelSize: 20
                            font.weight: Font.Bold
                            color: textPrimary
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    ToolButton {
                        icon.source: "qrc:/icons/chevron-right.svg"
                        onClicked: statsDialog.open()
                    }
                }
            }

            // Следующее ТО
            Rectangle {
                Layout.fillWidth: true
                Layout.margins: 16
                Layout.topMargin: 0
                height: 120
                color: "white"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 8

                    Label {
                        text: "СЛЕДУЮЩЕЕ ТО"
                        font.pixelSize: 12
                        color: textSecondary
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        ColumnLayout {
                            Layout.fillWidth: true

                            Label {
                                text: "Замена масла"
                                font.pixelSize: 16
                                font.weight: Font.Medium
                            }

                            Label {
                                text: "Осталось: 500 км"
                                font.pixelSize: 14
                                color: colorWarning
                            }
                        }

                        Label {
                            text: "5 500 ₽"
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            color: textPrimary
                        }
                    }

                    // Прогресс-бар
                    Rectangle {
                        Layout.fillWidth: true
                        height: 8
                        radius: 4
                        color: "#E0E0E0"

                        Rectangle {
                            width: parent.width * 0.95
                            height: parent.height
                            radius: 4
                            color: colorWarning
                        }
                    }

                    Label {
                        text: "Прогноз: 01.03.2026 (на основе " + avgMileage + " км/мес)"
                        font.pixelSize: 11
                        color: textSecondary
                    }
                }
            }

            // Заголовок предстоящих работ
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 16
                Layout.bottomMargin: 8

                Label {
                    text: "ПРЕДСТОЯЩИЕ РАБОТЫ"
                    font.pixelSize: 12
                    color: textSecondary
                    Layout.fillWidth: true
                }

                Label {
                    text: upcomingModel.count + " шт."
                    font.pixelSize: 12
                    color: textSecondary
                }
            }

            // Список предстоящих работ
            Repeater {
                model: upcomingModel

                Rectangle {
                    width: mainColumn.width
                    height: 72
                    color: "white"

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: "#E0E0E0"
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16

                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: index === 0 ? colorWarning : (index === 1 ? colorSuccess : textSecondary)
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: 8

                            Label {
                                text: name
                                font.pixelSize: 16
                                font.weight: index === 0 ? Font.Medium : Font.Normal
                            }

                            Label {
                                text: "Осталось: " + remaining + " км"
                                font.pixelSize: 12
                                color: index === 0 ? colorWarning : textSecondary
                            }
                        }

                        Label {
                            text: cost.toLocaleString() + " ₽"
                            font.pixelSize: 14
                            color: textPrimary
                        }

                        ToolButton {
                            icon.source: "qrc:/icons/check.svg"
                            icon.color: colorSuccess
                            onClicked: completeDialog.open()
                        }
                    }

                    // Свайп-действия (упрощенно)
                    MouseArea {
                        anchors.fill: parent
                        onPressAndHold: deleteDialog.open()
                    }
                }
            }

            // Заголовок последних работ
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 16
                Layout.topMargin: 24
                Layout.bottomMargin: 8

                Label {
                    text: "ПОСЛЕДНИЕ РАБОТЫ"
                    font.pixelSize: 12
                    color: textSecondary
                    Layout.fillWidth: true
                }

                ToolButton {
                    text: "Вся история"
                    font.pixelSize: 12
                    Material.foreground: colorPrimary
                    onClicked: historyDialog.open()
                }
            }

            // Список последних работ
            Repeater {
                model: historyModel

                Rectangle {
                    width: parent.width
                    height: 64
                    color: "white"

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: "#E0E0E0"
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16

                        Label {
                            text: "✅"
                            font.pixelSize: 20
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: 8

                            Label {
                                text: name
                                font.pixelSize: 16
                            }

                            Label {
                                text: date + " • " + mileage.toLocaleString(
                                          ) + " км"
                                font.pixelSize: 12
                                color: textSecondary
                            }
                        }

                        Label {
                            text: cost.toLocaleString() + " ₽"
                            font.pixelSize: 14
                            color: textPrimary
                        }

                        ToolButton {
                            icon.source: "qrc:/icons/repeat.svg"
                            onClicked: repeatDialog.open()
                        }
                    }
                }
            }

            // Нижний отступ
            Item {
                height: 16
                width: parent.width
            }
        }
    }

    // ScrollBar для Flickable
    ScrollBar.vertical: ScrollBar {
        parent: flickable
        policy: ScrollBar.AsNeeded
    }
    */
}

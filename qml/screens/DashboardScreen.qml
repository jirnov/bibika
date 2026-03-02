import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: window
    width: 360
    height: 640
    visible: true

    // Цвета для светлой темы
    readonly property color colorPrimary: "#6200EE"
    readonly property color colorPrimaryDark: "#3700B3"
    readonly property color colorAccent: "#03DAC6"
    readonly property color colorWarning: "#FFA000"
    readonly property color colorSuccess: "#4CAF50"
    readonly property color textPrimary: "#212121"
    readonly property color textSecondary: "#757575"

    // Тестовые данные
    property int currentMileage: 123456
    property string lastUpdated: "24.02.2026"
    property int totalExpenses: 124500
    property int avgMileage: 1500

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

            // Шапка (AppBar)
            Rectangle {
                Layout.fillWidth: true
                height: 56
                color: colorPrimary

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16

                    Label {
                        text: "AutoReminder"
                        font.pixelSize: 20
                        font.weight: Font.Medium
                        color: "white"
                        Layout.fillWidth: true
                    }

                    ToolButton {
                        icon.source: "qrc:/icons/plus.svg"
                        icon.color: "white"
                        onClicked: addTodoDialog.open()
                    }

                    ToolButton {
                        icon.source: "qrc:/icons/settings.svg"
                        icon.color: "white"
                        onClicked: settingsScreen.visible = true
                    }
                }
            }

            // Баннер напоминания (появляется если прошло >7 дней)
            Rectangle {
                Layout.fillWidth: true
                height: 56
                color: colorWarning
                visible: true // true если напоминание активно

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
                    width: parent.width
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

    // Диалог обновления пробега
    Dialog {
        id: mileageDialog
        title: "Обновление пробега"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true
        anchors.centerIn: parent

        contentItem: ColumnLayout {
            spacing: 16
            implicitWidth: 300

            Label {
                text: "Введите текущий пробег (км):"
            }

            TextField {
                id: mileageInput
                Layout.fillWidth: true
                placeholderText: currentMileage.toLocaleString()
                inputMethodHints: Qt.ImhDigitsOnly
            }
        }

        onAccepted: {
            if (mileageInput.text) {
                currentMileage = parseInt(mileageInput.text)
                lastUpdated = new Date().toLocaleDateString()
            }
        }
    }

    // Диалог добавления ТО
    Dialog {
        id: addTodoDialog
        title: "Новое ТО"
        standardButtons: Dialog.Save | Dialog.Cancel
        modal: true
        anchors.centerIn: parent

        contentItem: ColumnLayout {
            spacing: 12
            implicitWidth: 300

            TextField {
                placeholderText: "Название работы"
                Layout.fillWidth: true
            }

            TextField {
                placeholderText: "Пробег для ТО (км)"
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            TextField {
                placeholderText: "Стоимость (₽)"
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            ComboBox {
                Layout.fillWidth: true
                model: ["Регулярное", "Разовое"]
            }
        }
    }

    // Диалог отметки выполнения
    Dialog {
        id: completeDialog
        title: "Отметить выполненным"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true
        anchors.centerIn: parent

        contentItem: ColumnLayout {
            spacing: 12
            implicitWidth: 300

            Label {
                text: "Замена масла"
                font.weight: Font.Medium
            }

            TextField {
                placeholderText: "Пробег при выполнении"
                text: currentMileage.toLocaleString()
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            RowLayout {
                Layout.fillWidth: true

                TextField {
                    placeholderText: "Запчасти"
                    Layout.fillWidth: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                TextField {
                    placeholderText: "Работа"
                    Layout.fillWidth: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }
            }

            ComboBox {
                Layout.fillWidth: true
                model: ["Создать следующее", "Отметить как разовое", "Создать копию"]
            }
        }
    }

    // Диалог удаления
    Dialog {
        id: deleteDialog
        title: "Подтверждение"
        standardButtons: Dialog.Yes | Dialog.No
        modal: true
        anchors.centerIn: parent

        Label {
            text: "Удалить это напоминание?"
        }
    }

    // Диалог статистики
    Dialog {
        id: statsDialog
        title: "Статистика расходов"
        standardButtons: Dialog.Close
        modal: true
        anchors.centerIn: parent

        contentItem: ColumnLayout {
            spacing: 16
            implicitWidth: 300
            implicitHeight: 200

            Label {
                text: "2026 год: 45 800 ₽"
                font.weight: Font.Medium
            }

            // Простой график (полосы)
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Янв: 8 200"
                        Layout.preferredWidth: 80
                    }
                    Rectangle {
                        height: 20
                        width: (8200 / 20000) * 150
                        color: colorPrimary
                        radius: 4
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Фев: 15 300"
                        Layout.preferredWidth: 80
                    }
                    Rectangle {
                        height: 20
                        width: (15300 / 20000) * 150
                        color: colorPrimary
                        radius: 4
                    }
                }
            }
        }
    }

    // Диалог истории
    Dialog {
        id: historyDialog
        title: "История ТО"
        standardButtons: Dialog.Close
        modal: true
        anchors.centerIn: parent

        contentItem: ColumnLayout {
            spacing: 12
            implicitWidth: 300
            implicitHeight: 300

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: historyModel
                delegate: RowLayout {
                    width: parent.width
                    height: 40

                    Label {
                        text: date
                        Layout.preferredWidth: 70
                    }
                    Label {
                        text: name
                        Layout.fillWidth: true
                    }
                    Label {
                        text: cost + " ₽"
                    }
                }
            }
        }
    }

    // Диалог повтора
    Dialog {
        id: repeatDialog
        title: "Повторить ТО"
        standardButtons: Dialog.Yes | Dialog.No
        modal: true
        anchors.centerIn: parent

        ColumnLayout {
            Label {
                text: "Создать новое напоминание на основе:"
            }
            Label {
                text: "Замена масла от 24.02.2026"
                font.weight: Font.Medium
            }
        }
    }

    // Экран настроек (упрощенно)
    Rectangle {
        id: settingsScreen
        anchors.fill: parent
        color: "white"
        visible: false
        z: 10

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                Layout.fillWidth: true
                height: 56
                color: colorPrimary

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16

                    ToolButton {
                        icon.source: "qrc:/icons/arrow-back.svg"
                        icon.color: "white"
                        onClicked: settingsScreen.visible = false
                    }

                    Label {
                        text: "Настройки"
                        font.pixelSize: 20
                        color: "white"
                        Layout.fillWidth: true
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    width: parent.width
                    spacing: 0

                    GroupBox {
                        title: "Автомобиль"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent

                            TextField {
                                placeholderText: "Марка"
                                Layout.fillWidth: true
                            }

                            TextField {
                                placeholderText: "Модель"
                                Layout.fillWidth: true
                            }
                        }
                    }

                    GroupBox {
                        title: "Пробег"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent

                            Switch {
                                text: "Еженедельные напоминания"
                                checked: true
                            }

                            RowLayout {
                                Label {
                                    text: "Время:"
                                }
                                Button {
                                    text: "10:00"
                                }
                            }
                        }
                    }

                    GroupBox {
                        title: "Данные"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent

                            Button {
                                text: "Экспорт данных"
                                Layout.fillWidth: true
                            }

                            Button {
                                text: "Сброс всех данных"
                                Layout.fillWidth: true
                                Material.foreground: "red"
                            }
                        }
                    }
                }
            }
        }
    }

    // ScrollBar для Flickable
    ScrollBar.vertical: ScrollBar {
        parent: flickable
        policy: ScrollBar.AsNeeded
    }
}

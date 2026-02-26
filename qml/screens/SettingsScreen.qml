import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: settingsScreen
    ColumnLayout {

        Label {
            text: "Окно настроек"
        }

        Label {
            text: "Выбор интервала для напоминаний о пробеге, по умолчаю 7 дней"
        }

        Label {
            text: "Выбор значка валюты из ComboBox: $, ₽"
        }

        Button {
            text: "Вернуться"
            onClicked: rootScreen.pop()
        }
    }
}

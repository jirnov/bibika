import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    signal clicked

    ColumnLayout {

        Label {
            text: "Окно настроек"
        }

        Label {
            text: "Выбор интервала для напоминаний о пробеге, по умолчаю 7 дней"
        }

        Button {
            text: "Вернуться"
            onClicked: root.clicked()
        }
    }
}

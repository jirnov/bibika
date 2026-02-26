import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: editServiceRecordScreen

    ColumnLayout {

        Label {
            text: "Окно добавления или редактирования записи"
        }

        Button {
            text: "Вернуться"
            onClicked: rootScreen.pop()
        }
    }
}

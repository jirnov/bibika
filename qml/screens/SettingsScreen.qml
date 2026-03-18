import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: parent ? parent.width : AppSettings.width
    height: parent ? parent.height : AppSettings.height

    signal clicked

    // TODO: Добавить пункт "период напоминаний об обновлении пробега": никогда, неделя, две, месяц, два, выключить
    ColumnLayout {
        anchors.fill: parent
        Layout.fillWidth: true
        Label {
            text: "Окно настроек"
            font.pixelSize: Style.fontSizeTitle
        }

        Label {
            text: "Выбор интервала для напоминаний о пробеге"
        }

        Label {
            text: "Выбор интервала км для повтора ТО: 15000"
        }

        Label {
            text: "Выбор кол-ва месяцев для повтора ТО: 12"
        }

        Label {
            text: "Повторно ввести данные о машине - кнопка с переходом на WelcomeScreen"
        }

        Label {
            text: "Светлая тема - тумблер"
        }

        Button {
            text: "Вернуться"
            onClicked: root.clicked()
        }

        Item {
            Layout.fillHeight: true
        }
    }
}

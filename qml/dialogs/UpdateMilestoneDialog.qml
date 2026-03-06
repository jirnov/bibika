import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Диалог обновления пробега"
    standardButtons: Dialog.Cancel | Dialog.Ok
    modal: true

    onOpened: {
        standardButton(Dialog.Ok).enabled = false
    }

    onAccepted: {
        //settings.mileage = parseInt(milestoneField.text)
        //settings.lastMileageUpdate = new Date()
    }
}

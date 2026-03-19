import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Диалог обновления пробега"
    standardButtons: Dialog.Cancel | Dialog.Ok
    modal: true

    property int currentMileage: 0

    signal mileageChanged(int newMileage)

    onAccepted: {
    }
}

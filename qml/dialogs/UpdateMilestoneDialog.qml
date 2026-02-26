import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Dialog {
    id: updateMilestoneDialog
    title: "Диалог обновления пробега"
    standardButtons: Dialog.Cancel | Dialog.Ok
    modal: true

    ColumnLayout {
        MileageTextField {
            id: mileageTextField
            initialMileage: settings.mileage
            width: 400

            onMileageChanged: function (success) {
                var okButton = updateMilestoneDialog.standardButton(Dialog.Ok)
                okButton.enabled = success
            }
        }
    }

    onOpened: {
        standardButton(Dialog.Ok).enabled = false
    }

    onAccepted: {
        settings.mileage = parseInt(milestoneField.text)
        settings.mileageDate = new Date()
    }
}

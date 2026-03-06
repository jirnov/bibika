import QtQuick
import QtQuick.Layouts

ColumnLayout {
    id: root
/*
    property int initialMileage: 0
    property string initialLabel: "Текущий пробег: %1 км"
    property string successLabel: "Пробег %1 км"
    property string warnLabel: "Пробег не изменился"
    property string errorLabel: "Похоже на ошибку"

    property color successColor: "green"
    property color errorColor: "orange"

    signal mileageChanged(bool success)

    Layout.maximumWidth: 200.0

    Label {
        text: initialLabel.arg(initialMileage)
    }
    TextField {
        id: textItem
        text: String(initialMileage)
        validator: IntValidator {
            bottom: 0
        }
        Component.onCompleted: {
            var textChangedHandler = function () {
                var newMileage = parseInt(textItem.text)
                var diffMileage = newMileage - initialMileage
                if (diffMileage < 0) {
                    labelItem.text = errorLabel
                    labelItem.color = errorColor
                    mileageChanged(false)
                } else if (diffMileage > 0) {
                    labelItem.text = successLabel.arg(parseInt(diffMileage))
                    labelItem.color = successColor
                    mileageChanged(true)
                } else {
                    labelItem.text = warnLabel
                    labelItem.color = errorColor
                    mileageChanged(false)
                }
                console.log(labelItem.text)
                console.log(labelItem.color)
            }
            textItem.textChanged.connect(textChangedHandler)
        }
    }
    Label {
        id: labelItem
        font.pixelSize: 10
        wrapMode: Text.Wrap
    }*/
}

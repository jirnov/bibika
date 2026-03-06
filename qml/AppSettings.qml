pragma Singleton
import QtCore
import QtQuick
import BibikaService

QtObject
{
    id: root

    property Settings _persistent : Settings {
        property string carInfoJson: ""
    }

    readonly property CarInfo carInfo: CarInfoBuilder.fromJSON(root._persistent.value("carInfoJson"), root)

    readonly property string carName: {
        if (!carInfo.isValid) {
            return "Безымянный"
        }
        return carInfo.brandName + " " + carInfo.modelName
    }

    function updateCarInfo(newCarInfo) {
        root._persistent.carInfoJson = newCarInfo.toJSON()
        root._persistent.sync()
    }
}

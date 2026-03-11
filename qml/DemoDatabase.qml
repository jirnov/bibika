import QtQuick

import BibikaService

ListModel {
    id: root
    ListElement {
        index: 0
        eventType: 1
        name: "Замена масла"
        notes: "Лить только Liqui Molly"
        price: 4500
        mileage: 0
        serviceDate: "2026-03-01"

        repeatAfterDistance: 15000
        hasRepeatAfterDistance: true

        repeatAfterMonths: 12
        hasRepeatAfterMonths: true
    }

    ListElement {
        index: 1
        eventType: 0
        name: "Замена лобового стекла"
        notes: "Тайваньское"
        price: 20000
        mileage: 5000
        serviceDate: "2026-03-02"
    }

    ListElement {
        index: 2
        eventType: 2
        name: "Покупка омывайки"
        notes: "Зелёная, -30"
        price: 200
        mileage: 5500
        serviceDate: "2026-03-01"
    }

    ListElement {
        index: 3
        eventType: 0
        name: "Капитальный ремонт двигателя"
        notes: "Ппц дорого"
        price: 500000
        mileage: 10000
    }

    ListElement {
        index: 4
        eventType: 0
        name: "Замена масла в коробке"
        notes: ""
        price: 15000
        mileage: 0
    }
}

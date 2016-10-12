import QtQuick 2.0

ListModel {
    ListElement {
        name: "Bismuth Bronze"
        components: [
            ListElement {
                name: "Zinc"
                min_percent: 20
                max_percent: 30
            },
            ListElement {
                name: "Copper"
                min_percent: 50
                max_percent: 65
            },
            ListElement {
                name: "Bismuth"
                min_percent: 10
                max_percent: 20
            }
        ]
    }
    ListElement {
        name: "Black Bronze"
        components: [
            ListElement {
                name: "Copper"
                min_percent: 50
                max_percent: 70
            },
            ListElement {
                name: "Silver"
                min_percent: 10
                max_percent: 25
            },
            ListElement {
                name: "Gold"
                min_percent: 10
                max_percent: 25
            }
        ]
    }
}

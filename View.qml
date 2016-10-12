import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    readonly property var images_directory: "images/"
    readonly property var materials: Materials {}
    readonly property var alloys: Alloys {}

    ListModel {
        id: components
    }

    RowLayout {
        id: alloy_panel

        height: 32
        anchors.right: parent.right
        anchors.left: parent.left

        Image {
            id: alloy_image

            Layout.maximumHeight: parent.height
            Layout.maximumWidth: parent.height
            Layout.fillHeight: true

            source: images_directory + "unknown.png"
            fillMode: Image.PreserveAspectFit
        }

        ComboBox {
            id: alloy
            model: alloys
            textRole: "name"

            Layout.maximumWidth: 150
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: {
                alloy.currentIndexChanged.connect(changedAlloy);
                changedAlloy();
            }

            function updateComponentsTable() {
                components.clear();
                for(var i = 0; i < alloys.count; ++i) {
                    var a = alloys.get(i);
                    if(a.name !== alloy.currentText)
                        continue;

                    for(var j = 0; j < a.components.count; ++j) {
                        var comp = a.components.get(j);

                        // TODO: сделать словарём для того чтобы сортировалось
                        var el = {
                            name: comp.name,
                            min_percent: comp.min_percent,
                            max_percent: comp.max_percent,
                            cur_percent: 0,
                            value: ""
                        };

                        components.append(el);
                    }
                }
            }

            function updateAlloyImage() {
                for(var i = 0; i < materials.count; ++i) {
                    var material = materials.get(i);
                    if(material.name === alloy.currentText) {
                        alloy_image.source = images_directory + material.image;
                        return;
                    }
                }

                alloy_image.source = images_directory + "unknown.png";
            }

            function changedAlloy() {
                updateAlloyImage();
                updateComponentsTable();
            }
        }

        Text {
            id: total_units
            text: "Total units: "

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }
    }

    TableView {
        id: components_table

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: alloy_panel.bottom
        anchors.left: parent.left

        model: components

        TableViewColumn {
            role: "name"
            title: "Name"
            width: 60
        }
        TableViewColumn {
            title: "Expression"
            width: 100

            delegate: TextInput {
                text: styleData.value
                onEditingFinished: {
                    if(styleData.row < 0) // при очистке components становится отрицательным
                        return;

                    var res = evalExpressionAndCheck();
                    components.setProperty(styleData.row, "value", res);

                    var sum_values = getSumValues();
                    updatePercents(sum_values);
                }

                function updatePercents(sum_values) {
                    for(var i = 0; i < components.count; ++i) {
                        var row = components.get(i);
                        var value = parseInt(row.value);

                        var percent = (value * 100) / sum_values;
                        if(sum_values === 0)
                            percent = 0;

                        percent = Math.round(percent * 10) / 10;

                        if(isNaN(percent) || percent <= 0)
                            percent = 0;

                        components.setProperty(i, "cur_percent", percent);
                    }
                }

                function getSumValues() {
                    var sum = 0;
                    for(var i = 0; i < components.count; ++i) {
                        var row = components.get(i);
                        var value = parseInt(row.value);

                        if(isNaN(value) || value <= 0)
                            return 0;

                        sum += value;
                    }

                    return sum;
                }

                function evalExpressionAndCheck() {
                    var res;

                    try {
                        res = eval(text);
                    }
                    catch(exc) {
                        if(exc instanceof SyntaxError)
                            res = exc.message;
                        else
                            res = "<error>";
                    }

                    if(!isNaN(res))
                        res = res.toString();
                    else if(typeof res === "undefined")
                        res = "0";

                    return res;
                }
            }
        }
        TableViewColumn {
            role: "value"
            title: "Value"
            width: 35
        }
        TableViewColumn {
            role: "min_percent"
            title: "Min%"
            width: 35
        }
        TableViewColumn {
            role: "cur_percent"
            title: "Cur%"
            width: 40

            delegate: Text {
                text: styleData.value
                color: {
                    if(styleData.row < 0) // при очистке components становится отрицательным
                        return "black";

                    var row = components.get(styleData.row);

                    if(isNaN(text) || text < 1
                            || text < row.min_percent || text > row.max_percent)
                        return "red";
                    return "green";
                }
            }
        }
        TableViewColumn {
            role: "max_percent"
            title: "Max%"
            width: 35
        }
    }
}

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import QtQuick.Controls.Styles 1.4

Item {
    ComboBox {
        id: alloy
        model: Alloys {}
        textRole: "name"
        height: 52

        style: ComboBoxStyle {
            label: Item {
                Image {
                    id: img
                    height: alloy.height
                    fillMode: Image.PreserveAspectFit
                    source: "images/unknown.png"

                    Component.onCompleted: {
                        alloy.currentIndexChanged.connect(changeImage)
                    }

                    property var materials: Materials {}

                    function changeImage() {
                        var directory = "images/";
                        console.log("242342");

                        for(var i = 0; i < materials.count; ++i) {
                            var material = materials.get(i);
                            if(material.name === alloy.currentText) {
                                img.source = directory + material.image;
                                return;
                            }
                        }

                        img.source = directory + "unknown.png";
                    }
                }

                Text {
                    anchors.left: img.right
                    anchors.top: img.top

                    height: alloy.height

                    text: control.editText

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}

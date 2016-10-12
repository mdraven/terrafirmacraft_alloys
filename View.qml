import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import QtQuick.Controls.Styles 1.4

Item {
    ComboBox {
        id: alloy
        model: Alloys {}
        textRole: "name"
    }
}

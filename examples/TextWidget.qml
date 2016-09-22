import QtQuick 2.0
import QtQuick.Layouts 1.1

ColumnLayout {
    property alias label: lbl.text
    property alias text: txt.text
    Text {
        id: lbl
        width: parent.width
        font.bold: true
    }
    Rectangle {
        border.width: 1
        border.color: "black"
        width: parent.width
        Layout.fillWidth: true
        Layout.minimumWidth: lbl.width
        height: txt.height
        TextInput {
            id: txt
            width: parent.width
            font.pointSize: 12
            selectByMouse: true
        }
    }
}

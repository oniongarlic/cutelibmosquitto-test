import QtQuick 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
    property alias label: lbl.text
    property alias text: txt.text
    spacing: 8
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

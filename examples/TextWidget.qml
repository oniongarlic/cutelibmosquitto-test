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
    TextInput {
        id: txt
        width: parent.width
        selectByMouse: true
        Layout.fillWidth: true
        Layout.minimumWidth: lbl.width
    }
}

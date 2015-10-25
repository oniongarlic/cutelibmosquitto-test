import QtQuick 2.5
import QtQuick.Controls 1.4
import org.tal.sopomygga 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: mqtt.connect();
            }
            MenuItem {
                text: qsTr("&Close")
                onTriggered: mqtt.disconnect();
            }
            MenuItem {
                text: qsTr("Clear input")
                onTriggered: dataModel.clear()
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    MQTT {
        id: mqtt;
        clientId: "talorg-test"
        onConnected: {
            console.debug("MQTT Connected!")
            dataModel.clear();
            subscribe("radio/yle/#")
            publish("test/app/online", 1, 2, true);
            setWill("test/app/online", 0, 2, true);
        }
        onDisconnected: {
            console.debug("MQTT Disconnected!")
        }
        onConnecting: {
            console.debug("Connecting...")
        }

        onMsg: {
            console.debug(topic)
            console.debug(data)

            dataModel.append({"topic": topic, "message": data})
        }
        onError: console.debug("Error!")
    }

    ListModel {
        id: dataModel
    }

    ListView {
        id: view
        anchors.fill: parent
        model: dataModel
        delegate: Row {
            width: parent.width
            height: 30
            spacing: 16;
            Label {
                text: topic                
            }
            Label {
                text: message                
            }
        }
    }

}


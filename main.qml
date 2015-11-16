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
                enabled: !mqtt.isConnected
                onTriggered: mqtt.connect();
            }
            MenuItem {
                text: qsTr("&Close")
                enabled: mqtt.isConnected
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
    statusBar: StatusBar {

    }

    MQTT {
        id: mqtt;
        clientId: "talorg-test"
        keepalive: 60;
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

            dataModel.insert(0, {"topic": topic, "message": data})
        }
        onError: console.debug("Error!")
    }

    ListModel {
        id: dataModel
    }

    Column {
        anchors.fill: parent
        ListView {
            id: view
            model: dataModel
            height: parent.height/1.5
            width: parent.width
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

        Row {
            width: parent.width
            TextInput {
                id: txtTopic
                width: parent.width/3
            }
            TextInput {
                id: txtData
                width: parent.width/3
            }
            Button {
                id: txtSend
                text: "Set"
                enabled: mqtt.isConnected

                onClicked: {
                    mqtt.publish(txtTopic.text, txtData.text, 0, 0);
                }
            }
        }
        Row {
            width: parent.width
            TextInput {
                id: subTopic
                width: parent.width/2
            }
            Button {
                id: btnSub
                text: "Subscribe"
                enabled: mqtt.isConnected

                onClicked: {
                    mqtt.subscribe(subTopic.text);
                }
            }
        }
    }

}


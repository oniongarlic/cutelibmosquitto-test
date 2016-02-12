import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import org.tal.sopomygga 1.0

ApplicationWindow {
    id: app
    visible: true
    width: 640
    height: 480
    title: qsTr("libsopomygga test application")

    property string hostname: "localhost"
    property string username: ""
    property string password: ""
    property string clientid: "talorg-test"

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                enabled: !mqtt.isConnected
                onTriggered: mqtt.connectToHost();
            }
            MenuItem {
                text: qsTr("&Reconnect")
                enabled: mqtt.isConnected
                onTriggered: mqtt.reconnectToHost();
            }
            MenuItem {
                text: qsTr("&Close")
                enabled: mqtt.isConnected
                onTriggered: mqtt.disconnectFromHost();
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
        RowLayout {
            anchors.fill: parent
            Label { text: mqtt.isConnected ? "Connected to broker" : "Disconnected" }
            Label { text: mqtt.messageCount }
        }
    }

    MQTT {
        id: mqtt;
        clientId: app.clientid
        keepalive: 60;
        hostname: app.hostname

        property int messageCount: 0

        onConnected: {
            console.debug("MQTT Connected!")
            dataModel.clear();
            subscribe("radio/yle/#")
            subscribe("sauna/#")
            publish("test/app/online", 1, 2, true);
            setWill("test/app/online", 0, 2, true);
        }
        onDisconnected: {
            console.debug("MQTT Disconnected!")
            messageCount=0;
        }
        onConnecting: {
            console.debug("Connecting...")
        }

        onMsg: {
            console.debug(topic)
            console.debug(data)

            messageCount++

            dataModel.updateOrInsert({"topic": topic, "message": data})
        }
        onError: console.debug("Error!")
    }

    ListModel {
        id: dataModel

        function updateOrInsert(data) {
            var topic=data.topic;

            for (var i=0;i<dataModel.count;i++) {
                var tmp=dataModel.get(i);
                if (tmp.topic==topic) {
                    dataModel.set(i, data);
                    return;
                }
            }
            dataModel.insert(0, data)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8
        RowLayout {
            width: parent.width
            TextInput {
                id: txtHostname
            }
            TextInput {
                id: txtID
            }
        }

        ListView {
            id: view
            model: dataModel
            //Layout.fillHeight: true
            Layout.minimumHeight: 120
            Layout.preferredHeight: 300
            width: parent.width
            delegate: Row {
                width: parent.width
                height: 30
                spacing: 16;
                Text {
                    color: "#205806"
                    text: topic
                }
                Text {
                    color: "#051460"
                    text: message
                }
            }
            onCountChanged: {
                console.debug("Items: "+count)
            }
        }

        RowLayout {
            width: parent.width
            spacing: 8
            TextInput {
                id: txtTopic
                text: "/test/topic"
                Layout.minimumWidth: 80
                Layout.fillWidth: true
            }
            TextInput {
                id: txtData
                text: "TestData 1 2 3"
                Layout.minimumWidth: 80
                Layout.fillWidth: true
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
        RowLayout {
            width: parent.width
            TextInput {
                id: subTopic
                Layout.minimumWidth: 80
                Layout.fillWidth: true
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


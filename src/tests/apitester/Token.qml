import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property bool busy: auth.status == SocialContentItem.Busy
    property bool canConnect: !busy && authHelper.token === "" && emailField.text !== ""
                              && passwordField.text !== "" && authHelper.locale !== ""
                              && authHelper.deviceId !== ""
    property bool canDisconnect: !busy && authHelper.token !== ""
    property bool canEdit: !busy && authHelper.token === ""
    anchors.fill: parent

    SocialContentItem {
        id: auth
        socialNetwork: facebook
        request: FacebookAuthRequest {
            id: authRequest
            email: emailField.text
            password: passwordField.text
            deviceId: authHelper.deviceId
            machineId: authHelper.machineId
        }
        builder: FacebookAuthContentBuilder {}
        onFinished: {
            if (ok) {
                if (authHelper.machineId === "") {
                    authHelper.machineId = auth.object.machine_id
                }
                authHelper.userId = auth.object.uid
                authHelper.sessionKey = auth.object.session_key
                authHelper.secret = auth.object.secret
                authHelper.token = auth.object.access_token
                authHelper.save()
            } else {
                errorLabel.text = auth.errorString
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        ScrollView {
            id: infoView
            width: 300
            Layout.minimumWidth: 200

            Flickable {
                width: parent.width
                contentHeight: infoViewContent.height + infoViewContent.anchors.margins

                Column {
                    id: infoViewContent
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 12
                    spacing: 8

                    GroupBox {
                        title: "Credentials"
                        anchors.left: parent.left; anchors.right: parent.right

                        ColumnLayout {
                            spacing: 3
                            anchors.fill: parent
                            TextField {
                                id: emailField
                                enabled: container.canEdit
                                placeholderText: "Email"
                                Layout.fillWidth: true
                            }
                            TextField {
                                id: passwordField
                                enabled: container.canEdit
                                placeholderText: "Password"
                                echoMode: TextInput.Password
                                Layout.fillWidth: true
                            }
                        }
                    }

                    GroupBox {
                        title: "Infos"
                        anchors.left: parent.left; anchors.right: parent.right

                        ColumnLayout {
                            spacing: 3
                            anchors.fill: parent
                            TextField {
                                id: localeField
                                enabled: container.canEdit
                                placeholderText: "Locale"
                                Layout.fillWidth: true
                                text: authHelper.locale
                                onEditingFinished: authHelper.locale = text
                            }
                            TextField {
                                id: deviceIdField
                                enabled: container.canEdit
                                placeholderText: "Device id"
                                Layout.fillWidth: true
                                text: authHelper.deviceId
                                onEditingFinished: authHelper.deviceId = text
                            }
                            TextField {
                                id: machineIdField
                                enabled: container.canEdit
                                placeholderText: "Machine id"
                                Layout.fillWidth: true
                                text: authHelper.machineId
                                onEditingFinished: authHelper.machineId = text
                            }
                        }
                    }

                    Column {
                        anchors.left: parent.left; anchors.right: parent.right
                        spacing: 3

                        Button {
                            id: generateDeviceId
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canConnect
                            text: "Generate device id"
                            onClicked: {
                                authHelper.machineId = authHelper.generateMachineId()
                            }
                        }
                        Button {
                            id: connect
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canConnect
                            text: "Connect"
                            onClicked: {
                                authHelper.save()
                                auth.load()
                            }
                        }
                        Button {
                            id: disconnect
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canDisconnect
                            text: "Disconnect"
                        }
                    }

                    Label {
                        id: errorLabel
                        anchors.left: parent.left; anchors.right: parent.right
                        color: "red"
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }

        ScrollView {
            id: tokenView
            Layout.minimumWidth: 200

            Flickable {
                width: parent.width
                contentHeight: tokenViewContent.height + tokenViewContent.anchors.margins
                Column {
                    id: tokenViewContent
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 12
                    spacing: 8

                    GroupBox {
                        anchors.left: parent.left; anchors.right: parent.right
                        title: "User id"
                        TextField {
                            placeholderText: "User id"
                            anchors.fill: parent
                            readOnly: true
                            text: authHelper.userId
                        }
                    }
                    GroupBox {
                        anchors.left: parent.left; anchors.right: parent.right
                        title: "Session key"
                        TextField {
                            placeholderText: "Session key"
                            anchors.fill: parent
                            readOnly: true
                            text: authHelper.sessionKey
                        }
                    }
                    GroupBox {
                        anchors.left: parent.left; anchors.right: parent.right
                        title: "Secret"
                        TextField {
                            placeholderText: "Secret"
                            anchors.fill: parent
                            readOnly: true
                            text: authHelper.secret
                        }
                    }
                    GroupBox {
                        anchors.left: parent.left; anchors.right: parent.right
                        title: "Token"
                        TextField {
                            id: tokenField
                            anchors.fill: parent
                            readOnly: true
                            text: authHelper.token
                        }
                    }
                }
            }
        }
    }
}


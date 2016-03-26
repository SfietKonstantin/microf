import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property bool busy: login.status === ViewController.Busy
    property bool canGenerateDeviceId: !busy && authHelper.accessToken === "" && authHelper.deviceId === ""
    property bool canLogin: !busy && authHelper.accessToken === "" && emailField.text !== ""
                            && passwordField.text !== "" && authHelper.deviceId !== ""
    property bool canLogout: !busy && authHelper.accessToken !== ""
    property bool canEdit: !busy && authHelper.accessToken === ""
    anchors.fill: parent

    SessionViewItem {
        id: session
        controller: SessionController {
            id: login
            facebook: fb
            email: authHelper.email
            password: passwordField.text
            deviceId: authHelper.deviceId
            machineId: authHelper.machineId
            onFinished: {
                if (authHelper.machineId === "") {
                    authHelper.machineId = session.item.machineId
                }
                authHelper.userId = session.item.userId
                authHelper.sessionKey = session.item.sessionKey
                authHelper.secret = session.item.secret
                authHelper.accessToken = session.item.accessToken
                authHelper.save()
            }
            onError: {
                console.debug(login.errorMessage)
                errorLabel.text = login.errorMessage
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        ScrollView {
            id: infoView
            width: 400
            Layout.minimumWidth: 300

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
                                Layout.fillWidth: true
                                placeholderText: "Email"
                                text: authHelper.email
                                onEditingFinished: authHelper.email = text

                            }
                            TextField {
                                id: passwordField
                                enabled: container.canEdit
                                Layout.fillWidth: true
                                placeholderText: "Password"
                                echoMode: TextInput.Password
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
                                id: deviceIdField
                                enabled: container.canEdit
                                Layout.fillWidth: true
                                placeholderText: "Device id"
                                text: authHelper.deviceId
                                onEditingFinished: authHelper.deviceId = text
                            }
                            TextField {
                                id: machineIdField
                                enabled: container.canEdit
                                Layout.fillWidth: true
                                placeholderText: "Machine id"
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
                            enabled: container.canGenerateDeviceId
                            text: "Generate device id"
                            onClicked: {
                                authHelper.deviceId = authHelper.generateDeviceId()
                            }
                        }
                        Button {
                            id: loginButton
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canLogin
                            text: "Login"
                            onClicked: {
                                authHelper.save()
                                login.login()
                            }
                        }
                        Button {
                            id: logoutButton
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canLogout
                            text: "Logout"
                            onClicked: {
                                login.logout(authHelper.accessToken)
                            }
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
                            placeholderText: "Token"
                            anchors.fill: parent
                            readOnly: true
                            text: authHelper.accessToken
                        }
                    }
                }
            }
        }
    }
}

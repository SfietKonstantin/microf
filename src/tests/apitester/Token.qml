import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property bool busy: login.status == SocialContentItem.Busy || logout == SocialContentItem.Busy
    property bool canLogin: !busy && authHelper.accessToken === "" && emailField.text !== ""
                            && passwordField.text !== "" && authHelper.deviceId !== ""
    property bool canLogout: !busy && authHelper.accessToken !== ""
    property bool canEdit: !busy && authHelper.accessToken === ""
    anchors.fill: parent

    SocialContentItem {
        id: login
        socialNetwork: facebook
        request: FacebookLoginRequest {
            email: authHelper.email
            password: passwordField.text
            deviceId: authHelper.deviceId
            machineId: authHelper.machineId
        }
        builder: FacebookLoginContentBuilder {}
        onFinished: {
            if (ok) {
                if (authHelper.machineId === "") {
                    authHelper.machineId = login.object.machine_id
                }
                authHelper.userId = login.object.uid
                authHelper.sessionKey = login.object.session_key
                authHelper.secret = login.object.secret
                authHelper.accessToken = login.object.access_token
                authHelper.save()
            } else {
                errorLabel.text = login.errorString
            }
        }
    }

    SocialContentItem {
        id: logout
        socialNetwork: facebook
        request: FacebookLogoutRequest {}
        builder: FacebookConfirmationContentBuilder {}
        onFinished: {
            if (ok) {
                authHelper.logout()
            } else {
                errorLabel.text = login.errorString
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
                            enabled: container.canLogin
                            text: "Generate device id"
                            onClicked: {
                                authHelper.machineId = authHelper.generateMachineId()
                            }
                        }
                        Button {
                            id: loginButton
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canLogin
                            text: "Login"
                            onClicked: {
                                authHelper.save()
                                login.load()
                            }
                        }
                        Button {
                            id: logoutButton
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canLogout
                            text: "Logout"
                            onClicked: {
                                logout.load()
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
                            id: tokenField
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


import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property bool busy: item.status === SocialNetworkStatus.Busy || model.status === SocialNetworkStatus.Busy
    property bool canQuery: !busy && authHelper.accessToken !== ""
    property int type: RequestHelperModel.Invalid
    property var request: null
    anchors.fill: parent

    SocialContentItem {
        id: item
        socialNetwork: facebook
        request: container.request
        builder: FacebookItemBuilder {
            id: itemBuilder
        }
        onFinished: {
            if (!ok) {
                errorLabel.text = model.errorString
            } else {
                itemDisplay.refresh()
            }
        }
    }

    SocialContentModel {
        id: model
        socialNetwork: facebook
        request: container.request
        builder: FacebookModelBuilder {
            id: modelBuilder
        }
        onFinished: {
            if (!ok) {
                errorLabel.text = model.errorString
            }
        }
    }

    RequestHelperModel {
        id: requestModel
    }

    RequestPropertyHelperModel {
        id: requestPropertyModel
        request: container.request
    }

    IntValidator {
        id: intValidator
        bottom: 0
    }

    DoubleValidator {
        id: doubleValidator
        bottom: 0
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
                        title: "Request"
                        anchors.left: parent.left; anchors.right: parent.right

                        Column {
                            spacing: 3
                            anchors.left: parent.left; anchors.right: parent.right
                            ComboBox {
                                anchors.left: parent.left; anchors.right: parent.right
                                id: typeField
                                function setRequest() {
                                    container.request = requestModel.request(currentIndex)
                                    container.type = requestModel.type(currentIndex)
                                }
                                textRole: "text"
                                enabled: !container.busy
                                Layout.fillWidth: true
                                currentIndex: 0
                                onCurrentIndexChanged: setRequest()
                                Component.onCompleted: setRequest()
                                model: requestModel
                            }

                            Repeater {
                                id: repeater
                                anchors.left: parent.left; anchors.right: parent.right
                                model: requestPropertyModel
                                delegate: TextField {
                                    function save() {
                                        requestPropertyModel.save(model.index, text)
                                    }

                                    width: repeater.width
                                    placeholderText: model.name
                                    Component.onCompleted: {
                                        if (model.name === "userId") {
                                            text = authHelper.userId
                                            save()
                                        } else {
                                            text = model.value
                                        }
                                    }
                                    onEditingFinished: save()
                                    validator: {
                                        switch (model.type) {
                                        case RequestPropertyHelperModel.Int:
                                            return intValidator
                                        case RequestPropertyHelperModel.Double:
                                            return doubleValidator
                                        default:
                                            return null
                                        }
                                    }
                                    visible: model.type !== RequestPropertyHelperModel.Unknown
                                }
                            }

                            Label { text: "Properties to select" }
                            TextArea {
                                anchors.left: parent.left; anchors.right: parent.right
                                onTextChanged: {
                                    requestPropertyModel.setProperties(text, itemBuilder, modelBuilder)
                                }
                            }
                        }
                    }

                    Column {
                        id: buttons
                        function prepareLoad() {
                            switch (container.type) {
                            case RequestHelperModel.Object:
                                modelView.visible = false
                                itemView.visible = true
                                break
                            case RequestHelperModel.Model:
                                modelView.visible = true
                                itemView.visible = false
                                break
                            }
                        }

                        anchors.left: parent.left; anchors.right: parent.right
                        spacing: 3

                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery
                            text: "Request"
                            onClicked: {
                                buttons.prepareLoad()
                                switch (container.type) {
                                case RequestHelperModel.Object:
                                    item.load()
                                    break
                                case RequestHelperModel.Model:
                                    model.load()
                                    break
                                }
                            }
                        }
                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery && type === RequestHelperModel.Model && model.hasNext
                            text: "Request next"
                            onClicked: {
                                buttons.prepareLoad()
                                model.loadNext()
                            }
                        }
                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery && type === RequestHelperModel.Model && model.hasPrevious
                            text: "Request previous"
                            onClicked: {
                                buttons.prepareLoad()
                                model.loadPrevious()
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

        Item {
            Layout.minimumWidth: 200

            ListView {
                id: modelView
                anchors.top: parent.top; anchors.bottom: parent.bottom; anchors.margins: 12
                width: parent.width
                model: model
                spacing: 8
                delegate: Display {
                    width: modelView.width
                    object: model.object
                }
            }

            Flickable {
                id: itemView
                anchors.top: parent.top; anchors.bottom: parent.bottom; anchors.margins: 12
                width: parent.width
                contentHeight: itemDisplay.height

                Display {
                    id: itemDisplay
                    width: itemView.width
                    object: item.object
                }
            }
        }
    }
}


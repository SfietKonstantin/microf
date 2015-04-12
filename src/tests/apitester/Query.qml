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

        onFinished: {
            if (!ok) {
                errorLabel.text = model.errorString
            }
        }
    }

    SocialContentModel {
        id: model
        socialNetwork: facebook
        request: container.request
        builder: FacebookModelBuilder {}
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
                                anchors.left: parent.left; anchors.right: parent.right
                                model: requestPropertyModel
                                delegate: TextField {
                                    function save() {
                                        requestPropertyModel.save(model.index, text)
                                    }

                                    anchors.left: parent.left; anchors.right: parent.right
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
                        }
                    }

                    Column {
                        id: buttons
                        function prepareLoad() {
                            switch (container.type) {
                            case RequestHelperModel.Object:
                                modelView.visible = false
                                break
                            case RequestHelperModel.Model:
                                modelView.visible = true
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

        ListView {
            id: modelView
            Layout.minimumWidth: 200
            width: parent.width
            model: model
            delegate: Item {
                width: modelView.width
                height: rect.height + rect.anchors.margins

                Rectangle {
                    id: rect
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.margins: 12
                    anchors.verticalCenter: parent.verticalCenter
                    height: column.height + column.anchors.margins * 2

                    InfoHelper {
                        id: infoHelper
                        object: model.object
                    }

                    Column {
                        id: column
                        anchors.left: parent.left; anchors.right: parent.right
                        anchors.margins: 12
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 8
                        Label {
                            id: label
                            anchors.left: parent.left; anchors.right: parent.right
                            anchors.margins: 8
                            wrapMode: Text.WrapAnywhere
                            textFormat: Text.RichText
                            text: infoHelper.text
                        }

                        Repeater {
                            anchors.left: parent.left; anchors.right: parent.right
                            model: infoHelper.urls

                            Image {
                                anchors.left: parent.left; anchors.right: parent.right
                                source: model.modelData
                                fillMode: Image.PreserveAspectFit
                                visible: status === Image.Ready
                            }
                        }
                    }
                }
            }
        }
    }
}


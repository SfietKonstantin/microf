import QtQuick 2.0
import org.sfietkonstantin.microf 1.0
import QtQuick.Controls 1.2

Repeater {
    id: container
    property alias object: metaPropertyModel.object
    model: MetaPropertyHelperModel {
        id: metaPropertyModel
    }
    anchors.left: parent.left; anchors.right: parent.right
    delegate: TextField {
        function save() {
            metaPropertyModel.save(model.index, text)
        }

        width: container.width
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
            case MetaPropertyHelperModel.Int:
                return intValidator
            case MetaPropertyHelperModel.Double:
                return doubleValidator
            default:
                return null
            }
        }
        visible: container.type !== MetaPropertyHelperModel.Unknown
    }
}


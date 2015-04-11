import QtQuick 2.3
import QtQuick.Controls 1.2
import org.sfietkonstantin.microf 1.0

ApplicationWindow {
    id: app

    Facebook {
        id: facebook
        locale: authHelper.locale
    }

    AuthHelper {
        id: authHelper
    }

    visible: true
    title: "FB API browser"
    minimumWidth: 650
    minimumHeight: 500
    width: 650
    height: 500

    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem {
                text: "Quit"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
    }

    TabView {
        anchors.fill: parent
        anchors.margins: 12
        Tab {
            title: "Token"
            Token {}
        }
        Tab {
            id: queryPage
            title: "Query"
            enabled: authHelper.token !== ""
        }
    }
}


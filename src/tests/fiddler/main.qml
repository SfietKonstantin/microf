import QtQuick 2.5
import QtQuick.Controls 1.4
import org.sfietkonstantin.microf 1.0

ApplicationWindow {
    id: app

    Facebook {
        id: fb
        userId: authHelper.userId
        sessionKey: authHelper.sessionKey
        secret: authHelper.secret
        accessToken: authHelper.accessToken
    }

    AuthHelper {
        id: authHelper
    }

    visible: true
    title: "FB Fiddler"
    minimumWidth: 1000
    minimumHeight: 700
    width: 1000
    height: 700

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
            title: "Session"
            SessionTab {}
        }
        Tab {
            title: "Test"
            TestTab {}
        }
    }
}


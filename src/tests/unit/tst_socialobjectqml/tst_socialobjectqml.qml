import QtQuick 2.0
import QtTest 1.0
import org.sfietkonstantin.microf 1.0
import org.sfietkonstantin.microf.test 1.0

TestCase {
    id: test
    name: "TestSocialObject"

    SocialObjectManager {
        id: manager
    }

    Connections {
        target: manager.object
        onTest4Changed: ++test.count
    }

    property var test3: manager.object.test3
    property int count: 0

    function test_simple() {
        compare(manager.object.test, null);
        manager.setProperty("test", 12);
        compare(manager.object.test, 12);

        manager.setProperty("test2", true);
        compare(manager.object.test2, true);
    }

    function test_binding() {
        compare(test.test3, null);
        manager.setProperty("test3", "Hello World !");
        compare(test.test3, "Hello World !");
    }

    function test_signal() {
        compare(manager.object.test4, null);
        compare(test.count, 0);
        manager.setProperty("test4", 1234);
        compare(test.count, 1);
    }
}


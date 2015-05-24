/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <socialobject.h>
#include <socialobject_p.h>

class SignalSpy: public QObject
{
    Q_OBJECT
public:
    explicit SignalSpy(QObject *parent = 0);
    int count() const;
    QMetaMethod incrementMeta() const;
public slots:
    void increment();
private:
    int m_count;
};

SignalSpy::SignalSpy(QObject *parent)
    : QObject(parent), m_count(0)
{
}

int SignalSpy::count() const
{
    return m_count;
}

QMetaMethod SignalSpy::incrementMeta() const
{
    return metaObject()->method(metaObject()->indexOfMethod("increment()"));
}

void SignalSpy::increment()
{
    ++m_count;
}

class TstSocialObject: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSocialObject();
};

void TstSocialObject::testSocialObject()
{
    SocialObject *socialObject = new SocialObject(this);
    const QMetaObject *meta = socialObject->metaObject();

    // Check dynamically created property
    int index = meta->indexOfProperty("test");
    QVERIFY(index >= 0);
    QMetaProperty testProperty = meta->property(index);
    QVariant value = testProperty.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::QObjectStar);
    QCOMPARE(value.value<QObject *>(), static_cast<QObject *>(0));

    // Check dynamically set property
    SignalSpy spy;
    QMetaMethod signal = testProperty.notifySignal();
    connect(socialObject, signal, &spy, spy.incrementMeta());

    QVERIFY(meta->indexOfSignal(signal.methodSignature()) >= 0);
    SocialObjectPrivate::setProperty(socialObject, "test", QVariant(3));
    QCOMPARE(spy.count(), 1);
    value = testProperty.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::Int);
    QCOMPARE(value.toInt(), 3);

    // Check dynamically set list
    QVariantList list;
    list << QVariant(true) << QVariant(QString("test")) << QVariant(123);
    SocialObjectPrivate::setProperty(socialObject, "list", QVariant(list));
    index = meta->indexOfProperty("list");
    QVERIFY(index >= 0);
    QMetaProperty listProperty = meta->property(index);
    value = listProperty.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::QVariantList);
    QCOMPARE(value.toList(), list);

    // Check clear
    SocialObjectPrivate::clear(socialObject);
    QCOMPARE(spy.count(), 2);
    value = testProperty.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::QObjectStar);
    QCOMPARE(value.value<QObject *>(), static_cast<QObject *>(0));

    SocialObjectPrivate::clear(socialObject); // Clear again to test null QObject clearing
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::QObjectStar);
    QCOMPARE(value.value<QObject *>(), static_cast<QObject *>(0));

    QPointer<QObject> testObject = QPointer<QObject>(new QObject(socialObject));
    SocialObjectPrivate::setProperty(socialObject, "test", QVariant::fromValue(testObject.data()));
    value = testProperty.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::QObjectStar);
    QCOMPARE(value.value<QObject *>(), testObject.data());

    SocialObjectPrivate::clear(socialObject); // Clear to test QObject clearing
    value = testProperty.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::QObjectStar);
    QCOMPARE(value.value<QObject *>(), static_cast<QObject *>(0));
    QTest::qWait(200); // Let the QPointer receive the delete signal
    QVERIFY(testObject.isNull());

    // Check setting a new property
    SocialObjectPrivate::setProperty(socialObject, "test2", QVariant(true));
    index = meta->indexOfProperty("test2");
    QMetaProperty test2Property = meta->property(index);
    value = test2Property.read(socialObject);
    QCOMPARE(static_cast<QMetaType::Type>(value.type()), QMetaType::Bool);
    QCOMPARE(value.toBool(), true);
}

QTEST_MAIN(TstSocialObject)

#include "tst_socialobject.moc"

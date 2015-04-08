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

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>
#include <QtQml/qqml.h>
#include <QtQuickTest/QtQuickTest>
#include <socialobject.h>
#include <socialobject_p.h>

class SocialObjectManager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(SocialObject * object READ object CONSTANT)
public:
    explicit SocialObjectManager(QObject *parent = 0);
    SocialObject * object() const;
    void setObject(SocialObject *object);
    Q_INVOKABLE void setProperty(const QString &key, const QVariant &value);
signals:
    void objectChanged();
private:
    SocialObject *m_object;
};

SocialObjectManager::SocialObjectManager(QObject *parent)
    : QObject(parent), m_object(new SocialObject(this))
{
}

SocialObject * SocialObjectManager::object() const
{
    return m_object;
}

void SocialObjectManager::setProperty(const QString &key, const QVariant &value)
{
    SocialObjectPrivate::setProperty(m_object, key.toLocal8Bit(), value);
}

int main(int argc, char **argv)
{
    qmlRegisterUncreatableType<SocialObject>("org.sfietkonstantin.microf", 1, 0, "SocialObject",
                                             "Cannot be created");
    qmlRegisterType<SocialObjectManager>("org.sfietkonstantin.microf.test", 1, 0, "SocialObjectManager");
    return quick_test_main(argc, argv, "tst_dynamicobjectqml", ":/");
}

#include "tst_socialobjectqml.moc"

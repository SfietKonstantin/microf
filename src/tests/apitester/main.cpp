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

#include <QtWidgets/QApplication>
#include <QtCore/QSettings>
#include <QtCore/QUuid>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <socialcontentitem.h>
#include <socialobject.h>
#include <facebook/facebook.h>
#include <facebook/facebookauthrequest.h>
#include <facebook/facebookauthcontentbuilder.h>

class AuthHelper: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(QString machineId READ machineId WRITE setMachineId NOTIFY machineIdChanged)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString sessionKey READ sessionKey WRITE setSessionKey NOTIFY sessionKeyChanged)
    Q_PROPERTY(QString secret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
public:
    explicit AuthHelper(QObject *parent = 0)
        : QObject(parent)
    {
        QSettings settings;
        m_locale = settings.value("General/locale").toString();
        m_deviceId = settings.value("General/deviceId").toString();
        m_machineId = settings.value("General/machineId").toString();
        m_userId = settings.value("General/userId").toString();
        m_sessionKey = settings.value("General/sessionKey").toString();
        m_secret = settings.value("General/secret").toString();
        m_token = settings.value("General/token").toString();
    }
    ~AuthHelper() {}
    Q_INVOKABLE static QString generateMachineId()
    {
        return QUuid::createUuid().toString().remove("{").remove("}");
    }
    QString locale() const { return m_locale; }
    void setLocale(const QString &locale)
    {
        if (m_locale != locale) {
            m_locale = locale;
            emit localeChanged();
        }
    }
    QString deviceId() const { return m_deviceId; }
    void setDeviceId(const QString &deviceId)
    {
        if (m_deviceId != deviceId) {
            m_deviceId = deviceId;
            emit deviceIdChanged();
        }
    }
    QString machineId() const { return m_machineId; }
    void setMachineId(const QString &machineId)
    {
        if (m_machineId != machineId) {
            m_machineId = machineId;
            emit machineIdChanged();
        }
    }
    QString userId() const { return m_userId; }
    void setUserId(const QString &userId)
    {
        if (m_userId != userId) {
            m_userId = userId;
            emit userIdChanged();
        }
    }
    QString sessionKey() const { return m_sessionKey; }
    void setSessionKey(const QString &sessionKey)
    {
        if (m_sessionKey != sessionKey) {
            m_sessionKey = sessionKey;
            emit sessionKeyChanged();
        }
    }
    QString secret() const { return m_secret; }
    void setSecret(const QString &secret)
    {
        if (m_secret != secret) {
            m_secret = secret;
            emit secretChanged();
        }
    }
    QString token() const { return m_token; }
    void setToken(const QString &token)
    {
        if (m_token != token) {
            m_token = token;
            emit tokenChanged();
        }
    }
public slots:
    void save()
    {
        QSettings settings;
        settings.setValue("General/locale", m_locale);
        settings.setValue("General/deviceId", m_deviceId);
        settings.setValue("General/machineId", m_machineId);
        settings.setValue("General/userId", m_userId);
        settings.setValue("General/sessionKey", m_sessionKey);
        settings.setValue("General/secret", m_secret);
        settings.setValue("General/token", m_token);
    }
signals:
    void localeChanged();
    void deviceIdChanged();
    void machineIdChanged();
    void userIdChanged();
    void sessionKeyChanged();
    void secretChanged();
    void tokenChanged();
private:
    QString m_locale;
    QString m_deviceId;
    QString m_machineId;
    QString m_userId;
    QString m_sessionKey;
    QString m_secret;
    QString m_token;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qmlRegisterUncreatableType<SocialObject>("org.sfietkonstantin.microf", 1, 0, "SocialObject", "Abstract type");
    qmlRegisterUncreatableType<SocialNetwork>("org.sfietkonstantin.microf", 1, 0, "SocialNetwork", "Abstract type");
    qmlRegisterUncreatableType<SocialRequest>("org.sfietkonstantin.microf", 1, 0, "SocialRequest", "Abstract type");
    qmlRegisterUncreatableType<SocialContentBuilder>("org.sfietkonstantin.microf", 1, 0, "SocialContentBuilder", "Abstract type");
    qmlRegisterType<Facebook>("org.sfietkonstantin.microf", 1, 0, "Facebook");
    qmlRegisterType<SocialContentItem>("org.sfietkonstantin.microf", 1, 0, "SocialContentItem");
    qmlRegisterType<FacebookAuthRequest>("org.sfietkonstantin.microf", 1, 0, "FacebookAuthRequest");
    qmlRegisterType<FacebookAuthContentBuilder>("org.sfietkonstantin.microf", 1, 0, "FacebookAuthContentBuilder");
    qmlRegisterType<AuthHelper>("org.sfietkonstantin.microf", 1, 0, "AuthHelper");
    app.setOrganizationName("microf");
    app.setApplicationName("apitester");
    QQmlApplicationEngine engine (QUrl("qrc:/main.qml"));
    Q_UNUSED(engine);
    return app.exec();
}

#include "main.moc"

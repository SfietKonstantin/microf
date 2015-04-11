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
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>
#include <QtCore/QUuid>
#include <QtCore/QUrlQuery>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <socialcontentitem.h>
#include <socialobject.h>
#include <facebook/facebook.h>
#include <facebook/facebookloginrequest.h>
#include <facebook/facebooklogincontentbuilder.h>
#include <facebook/facebooklogoutrequest.h>
#include <facebook/facebookconfirmationcontentbuilder.h>

class AuthHelper: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(QString machineId READ machineId WRITE setMachineId NOTIFY machineIdChanged)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString sessionKey READ sessionKey WRITE setSessionKey NOTIFY sessionKeyChanged)
    Q_PROPERTY(QString secret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
public:
    explicit AuthHelper(QObject *parent = 0)
        : QObject(parent)
    {
        QSettings settings;
        m_email = settings.value("General/email").toString();
        m_deviceId = settings.value("General/deviceId").toString();
        m_machineId = settings.value("General/machineId").toString();
        m_userId = settings.value("General/userId").toString();
        m_sessionKey = settings.value("General/sessionKey").toString();
        m_secret = settings.value("General/secret").toString();
        m_accessToken = settings.value("General/accessToken").toString();
    }
    ~AuthHelper() {}
    Q_INVOKABLE static QString generateMachineId()
    {
        return QUuid::createUuid().toString().remove("{").remove("}");
    }
    Q_INVOKABLE static QString parseUrlQuery(const QString &queryString)
    {
        QString result;
        QUrlQuery query(queryString);
        for (QPair<QString, QString> queryEntry : query.queryItems(QUrl::FullyDecoded)) {
            result.append("<p><b>");
            result.append(queryEntry.first);
            result.append("</b>");
            result.append(" = ");
            result.append(QUrl::fromPercentEncoding(queryEntry.second.toLocal8Bit()));
            result.append("</p>");
        }
        return result;
    }
    Q_INVOKABLE static QString formatJson(const QString &json)
    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(json.toLocal8Bit(), &error);
        if (error.error != QJsonParseError::NoError) {
            return error.errorString();
        }
        return document.toJson(QJsonDocument::Indented);
    }
    QString email() const { return m_email; }
    void setEmail(const QString &email)
    {
        if (m_email != email) {
            m_email = email;
            emit emailChanged();
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
    QString accessToken() const { return m_accessToken; }
    void setAccessToken(const QString &accessToken)
    {
        if (m_accessToken != accessToken) {
            m_accessToken = accessToken;
            emit accessTokenChanged();
        }
    }
public slots:
    void save()
    {
        QSettings settings;
        settings.setValue("General/email", m_email);
        settings.setValue("General/deviceId", m_deviceId);
        settings.setValue("General/machineId", m_machineId);
        settings.setValue("General/userId", m_userId);
        settings.setValue("General/sessionKey", m_sessionKey);
        settings.setValue("General/secret", m_secret);
        settings.setValue("General/accessToken", m_accessToken);
    }
    void logout()
    {
        setUserId(QString());
        setSessionKey(QString());
        setSecret(QString());
        setAccessToken(QString());
        save();
    }

signals:
    void emailChanged();
    void deviceIdChanged();
    void machineIdChanged();
    void userIdChanged();
    void sessionKeyChanged();
    void secretChanged();
    void accessTokenChanged();
private:
    QString m_email;
    QString m_deviceId;
    QString m_machineId;
    QString m_userId;
    QString m_sessionKey;
    QString m_secret;
    QString m_accessToken;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qmlRegisterUncreatableType<SocialNetwork>("org.sfietkonstantin.microf", 1, 0, "SocialNetwork", "Abstract type");
    qmlRegisterUncreatableType<SocialNetworkStatus>("org.sfietkonstantin.microf", 1, 0, "SocialNetworkStatus", "Abstract type");
    qmlRegisterUncreatableType<SocialNetworkError>("org.sfietkonstantin.microf", 1, 0, "SocialNetworkError", "Abstract type");
    qmlRegisterUncreatableType<SocialObject>("org.sfietkonstantin.microf", 1, 0, "SocialObject", "Abstract type");
    qmlRegisterUncreatableType<SocialRequest>("org.sfietkonstantin.microf", 1, 0, "SocialRequest", "Abstract type");
    qmlRegisterUncreatableType<SocialContentBuilder>("org.sfietkonstantin.microf", 1, 0, "SocialContentBuilder", "Abstract type");
    qmlRegisterType<Facebook>("org.sfietkonstantin.microf", 1, 0, "Facebook");
    qmlRegisterType<SocialContentItem>("org.sfietkonstantin.microf", 1, 0, "SocialContentItem");
    qmlRegisterType<FacebookLoginRequest>("org.sfietkonstantin.microf", 1, 0, "FacebookLoginRequest");
    qmlRegisterType<FacebookLoginContentBuilder>("org.sfietkonstantin.microf", 1, 0, "FacebookLoginContentBuilder");
    qmlRegisterType<FacebookLogoutRequest>("org.sfietkonstantin.microf", 1, 0, "FacebookLogoutRequest");
    qmlRegisterType<FacebookConfirmationContentBuilder>("org.sfietkonstantin.microf", 1, 0, "FacebookConfirmationContentBuilder");
    qmlRegisterType<AuthHelper>("org.sfietkonstantin.microf", 1, 0, "AuthHelper");
    app.setOrganizationName("microf");
    app.setApplicationName("apitester");
    QQmlApplicationEngine engine (QUrl("qrc:/main.qml"));
    Q_UNUSED(engine);
    return app.exec();
}

#include "main.moc"

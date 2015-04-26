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

#ifndef AUTHHELPER_H
#define AUTHHELPER_H

#include <QtCore/QObject>

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
    explicit AuthHelper(QObject *parent = 0);
    ~AuthHelper();
    Q_INVOKABLE static QString generateDeviceId();
    Q_INVOKABLE static QString parseUrlQuery(const QString &queryString);
    Q_INVOKABLE static QString formatJson(const QString &json);
    Q_INVOKABLE static QString fromBase64(const QString &base64);
    QString email() const;
    void setEmail(const QString &email);
    QString deviceId() const;
    void setDeviceId(const QString &deviceId);
    QString machineId() const;
    void setMachineId(const QString &machineId);
    QString userId() const;
    void setUserId(const QString &userId);
    QString sessionKey() const;
    void setSessionKey(const QString &sessionKey);
    QString secret() const;
    void setSecret(const QString &secret);
    QString accessToken() const;
    void setAccessToken(const QString &accessToken);
public slots:
    void save();
    void logout();
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



#endif // AUTHHELPER_H

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

#include "facebookloginrequest.h"
#include "socialrequest_p.h"
#include "facebook.h"
#include <QtCore/QLocale>
#include <QtCore/QCryptographicHash>
#include <QtCore/QUrlQuery>

static const int SIG_SUFFIX0 = 62;
static const char *SIG_SUFFIX1 = "f";
static const int SIG_SUFFIX2 = 8;
static const char *SIG_SUFFIX3 = "ce";
static const int SIG_SUFFIX4 = 9;
static const char *SIG_SUFFIX5 = "f";
static const int SIG_SUFFIX6 = 74;
static const char *SIG_SUFFIX7 = "b";
static const int SIG_SUFFIX8 = 12;
static const char *SIG_SUFFIX9 = "f";
static const int SIG_SUFFIX10 = 84;
static const char *SIG_SUFFIX11 = "c";
static const int SIG_SUFFIX12 = 123;
static const char *SIG_SUFFIX13 = "cc";
static const int SIG_SUFFIX14 = 23437;
static const char *SIG_SUFFIX15 = "a";
static const int SIG_SUFFIX16 = 4;
static const char *SIG_SUFFIX17 = "a";
static const int SIG_SUFFIX18 = 32;

static QString sigSuffix()
{
    QString returned;
    returned.append(QString::number(SIG_SUFFIX0))
            .append(SIG_SUFFIX1)
            .append(QString::number(SIG_SUFFIX2))
            .append(SIG_SUFFIX3)
            .append(QString::number(SIG_SUFFIX4))
            .append(SIG_SUFFIX5)
            .append(QString::number(SIG_SUFFIX6))
            .append(SIG_SUFFIX7)
            .append(QString::number(SIG_SUFFIX8))
            .append(SIG_SUFFIX9)
            .append(QString::number(SIG_SUFFIX10))
            .append(SIG_SUFFIX11)
            .append(QString::number(SIG_SUFFIX12))
            .append(SIG_SUFFIX13)
            .append(QString::number(SIG_SUFFIX14))
            .append(SIG_SUFFIX15)
            .append(QString::number(SIG_SUFFIX16))
            .append(SIG_SUFFIX17)
            .append(QString::number(SIG_SUFFIX18));
    return returned;
}

class FacebookLoginRequestPrivate: public SocialRequestPrivate
{
public:
    explicit FacebookLoginRequestPrivate(FacebookLoginRequest *q);
    QString email;
    QString password;
    QString deviceId;
    QString machineId;
};

FacebookLoginRequestPrivate::FacebookLoginRequestPrivate(FacebookLoginRequest *q)
    : SocialRequestPrivate(q)
{
}

FacebookLoginRequest::FacebookLoginRequest(QObject *parent)
    : SocialRequest(*(new FacebookLoginRequestPrivate(this)), parent)
{
}

SocialRequest::Type FacebookLoginRequest::type() const
{
    return SocialRequest::Post;
}

QString FacebookLoginRequest::email() const
{
    Q_D(const FacebookLoginRequest);
    return d->email;
}

void FacebookLoginRequest::setEmail(const QString &email)
{
    Q_D(FacebookLoginRequest);
    if (d->email != email) {
        d->email = email;
        emit emailChanged();
    }
}

QString FacebookLoginRequest::password() const
{
    Q_D(const FacebookLoginRequest);
    return d->password;
}

void FacebookLoginRequest::setPassword(const QString &password)
{
    Q_D(FacebookLoginRequest);
    if (d->password != password) {
        d->password = password;
        emit passwordChanged();
    }
}

QString FacebookLoginRequest::deviceId() const
{
    Q_D(const FacebookLoginRequest);
    return d->deviceId;
}

void FacebookLoginRequest::setDeviceId(const QString &deviceId)
{
    Q_D(FacebookLoginRequest);
    if (d->deviceId != deviceId) {
        d->deviceId = deviceId;
        emit deviceIdChanged();
    }
}

QString FacebookLoginRequest::machineId() const
{
    Q_D(const FacebookLoginRequest);
    return d->machineId;
}

void FacebookLoginRequest::setMachineId(const QString &machineId)
{
    Q_D(FacebookLoginRequest);
    if (d->machineId != machineId) {
        d->machineId = machineId;
        emit machineIdChanged();
    }
}

QNetworkRequest FacebookLoginRequest::createRequest(const SocialNetwork &socialNetwork,
                                                    const QByteArray &postData, Mode mode,
                                                    const QVariantMap &metadata) const
{
    Q_UNUSED(mode);
    Q_UNUSED(metadata);
    const Facebook *facebook = qobject_cast<const Facebook *>(&socialNetwork);
    if (!facebook) {
        return QNetworkRequest();
    }

    QNetworkRequest request (QUrl("https://b-api.facebook.com/method/auth.login"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Host", "b-api.facebook.com");
    request.setRawHeader("Connection", "Keep-Alive");
    request.setRawHeader("User-Agent", facebook->userAgent());
    request.setRawHeader("Content-Length", QByteArray::number(postData.size()));
    request.setRawHeader("X-FB-HTTP-Engine", "Apache");
    return request;
}

QByteArray FacebookLoginRequest::createPostData(const SocialNetwork &socialNetwork, Mode mode,
                                                const QVariantMap &metadata) const
{
    Q_D(const FacebookLoginRequest);
    Q_UNUSED(mode);
    Q_UNUSED(metadata);
    const Facebook *facebook = qobject_cast<const Facebook *>(&socialNetwork);
    if (!facebook) {
        return QByteArray();
    }

    QMap<QString, QString> parameters;
    parameters.insert("api_key", facebook->apiKey());
    parameters.insert("client_country_code", facebook->countryCode());
    parameters.insert("credentials_type", "password");
    parameters.insert("device_id", d->deviceId);
    parameters.insert("email", d->email);
    parameters.insert("error_detail_type", "button_with_disabled");
    parameters.insert("fb_api_caller_class", "com.facebook.katana.server.handler.Fb4aAuthHandler");
    parameters.insert("fb_api_req_friendly_name", "authenticate");
    parameters.insert("format", "json");
    parameters.insert("generate_session_cookies", "1");
    parameters.insert("locale", facebook->locale());
    if (d->machineId.isEmpty()) {
        parameters.insert("generate_machine_id", "1");
    } else {
        parameters.insert("machine_id", d->machineId);
    }
    parameters.insert("method", "auth.login");
    parameters.insert("password", d->password);

    QByteArray sig;
    for (const QString &key : parameters.keys()) {
        sig.append(key.toUtf8());
        sig.append("=");
        sig.append(parameters.value(key).toUtf8());
    }
    sig.append(sigSuffix());
    QCryptographicHash hash (QCryptographicHash::Md5);
    hash.addData(sig);
    QByteArray sigHash = hash.result().toHex();
    parameters.insert("sig", sigHash);

    QUrlQuery query;
    for (const QString &key : parameters.keys()) {
        query.addQueryItem(key, parameters.value(key));
    }
    return query.toString(QUrl::FullyEncoded).toLocal8Bit();
}


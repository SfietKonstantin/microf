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

#include "facebook.h"
#include "socialnetwork_p.h"
#include <QtCore/QLocale>

static const char *USER_AGENT = "Dalvik/2.1.0 (Linux; U; Android 5.0.2; Android on Emulator) [FBAN/FB4A;FBAV/29.0.0.23.13;FBBV/7888989;FBDM/{density=1.5,width=480,height=800};FBLC/%1;FBCR/Android;FBPN/com.facebook.katana;FBDV/Full Android on Emulator;FBSV/5.0.2;FBOP/1;FBCA/armeabi-v7a:armeabi;]";
static const int API_KEY0 = 882;
static const char *API_KEY1 = "a";
static const int API_KEY2 = 8490361;
static const char *API_KEY3 = "da";
static const int API_KEY4 = 98702;
static const char *API_KEY5 = "bf";
static const int API_KEY6 = 97;
static const char *API_KEY7 = "a0";
static const int API_KEY8 = 21;
static const char *API_KEY9 = "ddc";
static const int API_KEY10 = 14;
static const char *API_KEY11 = "d";

class FacebookPrivate: public SocialNetworkPrivate
{
public:
    explicit FacebookPrivate(Facebook *q);
    void updateCountryCode();
    QString locale;
    QString countryCode;
    QString userId;
    QString sessionKey;
    QString secret;
    QString accessToken;
private:
    Q_DECLARE_PUBLIC(Facebook)
};

FacebookPrivate::FacebookPrivate(Facebook *q)
    : SocialNetworkPrivate(q)
{
    const QLocale &systemLocale = QLocale::system();
    countryCode = QLocale::countryToString(systemLocale.country());
    locale = QString("%1_%2").arg(QLocale::languageToString(systemLocale.language()), countryCode);
}

Facebook::Facebook(QObject *parent)
    : SocialNetwork(*(new FacebookPrivate(this)), parent)
{
}

QString Facebook::apiKey() const
{
    QString returned;
    returned.append(QString::number(API_KEY0))
            .append(API_KEY1)
            .append(QString::number(API_KEY2))
            .append(API_KEY3)
            .append(QString::number(API_KEY4))
            .append(API_KEY5)
            .append(QString::number(API_KEY6))
            .append(API_KEY7)
            .append(QString::number(API_KEY8))
            .append(API_KEY9)
            .append(QString::number(API_KEY10))
            .append(API_KEY11);
    return returned;
}

QString Facebook::locale() const
{
    Q_D(const Facebook);
    return d->locale;
}

QString Facebook::countryCode() const
{
    Q_D(const Facebook);
    return d->countryCode;
}

QString Facebook::userId() const
{
    Q_D(const Facebook);
    return d->userId;
}

void Facebook::setUserId(const QString &userId)
{
    Q_D(Facebook);
    if (d->userId != userId) {
        d->userId = userId;
        emit userIdChanged();
    }
}

QString Facebook::sessionKey() const
{
    Q_D(const Facebook);
    return d->sessionKey;
}

void Facebook::setSessionKey(const QString &sessionKey)
{
    Q_D(Facebook);
    if (d->sessionKey != sessionKey) {
        d->sessionKey = sessionKey;
        emit sessionKeyChanged();
    }
}

QString Facebook::secret() const
{
    Q_D(const Facebook);
    return d->secret;
}

void Facebook::setSecret(const QString &secret)
{
    Q_D(Facebook);
    if (d->secret != secret) {
        d->secret = secret;
        emit secretChanged();
    }
}

QString Facebook::accessToken() const
{
    Q_D(const Facebook);
    return d->accessToken;
}

void Facebook::setAccessToken(const QString &accessToken)
{
    Q_D(Facebook);
    if (d->accessToken != accessToken) {
        d->accessToken = accessToken;
        emit accessTokenChanged();
    }
}

QByteArray Facebook::userAgent() const
{
    Q_D(const Facebook);
    return QString(USER_AGENT).arg(d->locale.isEmpty() ? "" : d->locale).toUtf8();
}

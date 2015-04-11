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

#include "facebooklogincontentbuilder.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

static const char *UID_KEY = "uid";
static const char *SESSION_KEY_KEY = "session_key";
static const char *SECRET_KEY = "secret";
static const char *ACCESS_TOKEN_KEY = "access_token";

FacebookLoginContentBuilder::FacebookLoginContentBuilder(QObject *parent)
    : SocialContentBuilder(parent)
{
}

void FacebookLoginContentBuilder::build(SocialContentItem &contentItem,
                                       QNetworkReply::NetworkError error,
                                       const QString &errorString,
                                       const QByteArray &data)
{
    if (!error == QNetworkReply::NoError) {
        qWarning() << data;
        setError(contentItem, SocialNetworkError::Network, errorString);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(data);
    if (!document.isObject()) {
        setError(contentItem, SocialNetworkError::Data, "Cannot convert to JSON");
        return;
    }

    QVariantMap properties;
    const QJsonObject &object = document.object();
    if (object.contains(UID_KEY)) {
        const QJsonValue &value = object.value(UID_KEY);
        if (value.isDouble()) {
            properties.insert(UID_KEY, QString::number(value.toInt()));
        }
    }
    if (object.contains(SESSION_KEY_KEY)) {
        const QJsonValue &value = object.value(SESSION_KEY_KEY);
        if (value.isString()) {
            properties.insert(SESSION_KEY_KEY, value.toString());
        }
    }
    if (object.contains(SECRET_KEY)) {
        const QJsonValue &value = object.value(SECRET_KEY);
        if (value.isString()) {
            properties.insert(SECRET_KEY, value.toString());
        }
    }
    if (object.contains(ACCESS_TOKEN_KEY)) {
        const QJsonValue &value = object.value(ACCESS_TOKEN_KEY);
        if (value.isString()) {
            properties.insert(ACCESS_TOKEN_KEY, value.toString());
        }
    }
    setObject(contentItem, properties);
}

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

static const char *ERROR_DATA_KEY = "error_data";
static const char *ERROR_MESSAGE_KEY = "error_message";
static const char *UID_KEY = "uid";
static const char *SESSION_KEY_KEY = "session_key";
static const char *SECRET_KEY = "secret";
static const char *ACCESS_TOKEN_KEY = "access_token";
static const char *MACHINE_ID_KEY = "machine_id";

FacebookLoginContentBuilder::FacebookLoginContentBuilder(QObject *parent)
    : SocialContentItemBuilder(parent)
{
}

void FacebookLoginContentBuilder::build(SocialContentItem &contentItem,
                                       QNetworkReply::NetworkError error,
                                       const QString &errorMessage,
                                       const QByteArray &data, const QVariantMap &metadata)
{
    Q_UNUSED(metadata);
    if (error == QNetworkReply::NoError) {
        qWarning() << data;
        setError(contentItem, SocialNetworkError::Network, errorMessage);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(data);
    if (!document.isObject()) {
        setError(contentItem, SocialNetworkError::Data, "Cannot convert to JSON");
        return;
    }

    QVariantMap properties;
    const QJsonObject &object = document.object();
    // Check if we got an error
    if (object.contains(ERROR_DATA_KEY)) {
        QString errorData = object.value(ERROR_DATA_KEY).toString();
        QJsonDocument errorDocument = QJsonDocument::fromJson(errorData.toLocal8Bit());
        if (!errorDocument.isObject()) {
            setError(contentItem, SocialNetworkError::Data, "Cannot convert to JSON");
            return;
        }

        const QJsonObject &errorObject = errorDocument.object();
        if (!errorObject.contains(ERROR_MESSAGE_KEY)) {
            setError(contentItem, SocialNetworkError::Data,
                     "Cannot find error_title in error message");
            return;
        }
        QString errorTitle = errorObject.value(ERROR_MESSAGE_KEY).toString();
        setError(contentItem, SocialNetworkError::SocialNetwork, errorTitle);
        return;
    }

    const QJsonValue &accessToken = object.value(ACCESS_TOKEN_KEY);
    if (!accessToken.isString()) {
        setError(contentItem, SocialNetworkError::Data, "Cannot find access_token in JSON");
        return;
    }
    const QJsonValue &uid = object.value(UID_KEY);
    if (!uid.isDouble() && !uid.isString()) {
        setError(contentItem, SocialNetworkError::Data, "Cannot find uid in JSON");
        return;
    }

    properties.insert(ACCESS_TOKEN_KEY, accessToken.toString());
    if (uid.isDouble()) {
        properties.insert(UID_KEY, QString::number(uid.toVariant().toInt()));
    } else if (uid.isString()) {
        properties.insert(UID_KEY, uid.toString());
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
    if (object.contains(MACHINE_ID_KEY)) {
        const QJsonValue &value = object.value(MACHINE_ID_KEY);
        if (value.isString()) {
            properties.insert(MACHINE_ID_KEY, value.toString());
        }
    }

    setObject(contentItem, properties);
}

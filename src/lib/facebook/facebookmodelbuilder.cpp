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

#include "facebookmodelbuilder.h"
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

FacebookModelBuilder::FacebookModelBuilder(QObject *parent)
    : SocialContentModelBuilder(parent)
{
}

static void recursiveSetValues(const QJsonObject &object, const QString &prefix,
                               QVariantMap &properties)
{
    for (const QString &key : object.keys()) {
        const QJsonValue &value = object.value(key);
        QString realKey;
        if (!prefix.isEmpty()) {
            realKey = QString("%1_%2").arg(prefix, key);
        } else {
            realKey = key;
        }
        if (value.isBool()) {
            properties.insert(realKey, value.toBool());
        } else if (value.isDouble()) {
            properties.insert(realKey, value.toDouble());
        } else if (value.isObject()) {
            recursiveSetValues(value.toObject(), realKey, properties);
        } else if (value.isString()) {
            properties.insert(realKey, value.toString());
        }
    }
}

void FacebookModelBuilder::build(SocialContentModel &contentModel,
                                 QNetworkReply::NetworkError error, const QString &errorString,
                                 const QByteArray &data, const QVariantMap &metadata)
{
    if (error != QNetworkReply::NoError) {
        setError(contentModel, SocialNetworkError::Network, errorString);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(data);
    if (!document.isObject()) {
        setError(contentModel, SocialNetworkError::Data, "Cannot convert to JSON");
        return;
    }

    // First check metadata
    const QJsonObject &root = document.object();
    QString userId = metadata.value("userId").toString();

    if (!root.contains(userId)) {
        setError(contentModel, SocialNetworkError::Data, "Cannot find userId object");
        return;
    }

    const QJsonObject &userIdData = root.value(userId).toObject();
    QString type = metadata.value("type").toString();

    if (userIdData.value("__type__").toObject().value("name").toString() != type) {
        setError(contentModel, SocialNetworkError::Data, "Cannot match the type");
        return;
    }

    const QString &dataRoot = metadata.value("dataRoot").toString();
    if (!userIdData.contains(dataRoot)) {
        setError(contentModel, SocialNetworkError::Data, "Cannot find data root object");
        return;
    }

    const QJsonObject &dataObject = userIdData.value(dataRoot).toObject();
    const QJsonArray &nodes = dataObject.value("nodes").toArray();
    const QJsonObject &pageInfo = dataObject.value("page_info").toObject();

    QList<QVariantMap> returnedData;
    for (const QJsonValue &value : nodes) {
        QVariantMap properties;
        recursiveSetValues(value.toObject(), QString(""), properties);
        returnedData.append(properties);
    }

    bool hasNextPage = pageInfo.value("has_next_page").toBool();
    QString startCursor = pageInfo.value("start_cursor").toString();
    QString endCursor = pageInfo.value("end_cursor").toString();

    QVariantMap newMetadata = metadata;
    newMetadata.insert("startCursor", startCursor);
    newMetadata.insert("endCursor", endCursor);
    setData(contentModel, returnedData, hasNextPage, false, newMetadata);
}


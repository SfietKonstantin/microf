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

#include "facebooknewsfeedmodelbuilder.h"
#include "abstractfacebookmodelbuilder_p.h"
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include "facebook_p.h"

FacebookNewsFeedModelBuilder::FacebookNewsFeedModelBuilder(QObject *parent)
    : AbstractFacebookModelBuilder(*(new AbstractFacebookModelBuilderPrivate(this)), parent)
{
}

void FacebookNewsFeedModelBuilder::build(SocialContentModel &contentModel,
                                         QNetworkReply::NetworkError error,
                                         const QString &errorMessage, const QByteArray &data,
                                         const QVariantMap &metadata)
{
    Q_D(AbstractFacebookModelBuilder);
    SocialNetworkError::type outError = SocialNetworkError::None;
    QString outErrorMessage;
    QString outErrorCode;

    QJsonObject root = FacebookPrivate::checkError(error, errorMessage, data, outError,
                                                   outErrorMessage, outErrorCode);
    if (outError != SocialNetworkError::None) {
        setError(contentModel, outError, outErrorMessage, outErrorCode);
        return;
    }

    if (!root.contains("viewer")) {
        setError(contentModel, SocialNetworkError::Data, "Cannot find viewer object");
        return;
    }

    const QJsonObject &viewer = root.value("viewer").toObject();
    if (!viewer.contains("news_feed")) {
        setError(contentModel, SocialNetworkError::Data, "Cannot find news_feed object");
        return;
    }

    const QJsonObject &dataObject = viewer.value("news_feed").toObject();
    const QJsonArray &nodes = dataObject.value("edges").toArray();
    const QJsonObject &pageInfo = dataObject.value("page_info").toObject();

    QList<QVariantMap> returnedData;
    for (const QJsonValue &value : nodes) {
        returnedData.append(FacebookPrivate::buildProperties(value.toObject(), d->properties()));
    }

    bool hasNextPage = pageInfo.value("has_next_page").toBool();
    QString startCursor = pageInfo.value("start_cursor").toString();
    QString endCursor = pageInfo.value("end_cursor").toString();

    QVariantMap newMetadata = metadata;
    newMetadata.insert("startCursor", startCursor);
    newMetadata.insert("endCursor", endCursor);
    setData(contentModel, returnedData, hasNextPage, false, newMetadata);
}


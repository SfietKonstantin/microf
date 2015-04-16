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
#include "socialcontentmodelbuilder_p.h"
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include "facebook_p.h"

class FacebookModelBuilderPrivate: public SocialContentModelBuilderPrivate
{
public:
    explicit FacebookModelBuilderPrivate(FacebookModelBuilder *q);
    static void properties_append(QQmlListProperty<FacebookProperty> *list, FacebookProperty *property);
    static int properties_count(QQmlListProperty<FacebookProperty> *list);
    static FacebookProperty * properties_at(QQmlListProperty<FacebookProperty> *list, int index);
    static void properties_clear(QQmlListProperty<FacebookProperty> *list);
    void setRawData(const QString &rawData);
private:
    void clear();
    QList<FacebookProperty *> m_properties;
    bool m_includeRawData;
    QString m_rawData;
    Q_DECLARE_PUBLIC(FacebookModelBuilder)
};

FacebookModelBuilderPrivate::FacebookModelBuilderPrivate(FacebookModelBuilder *q)
    : SocialContentModelBuilderPrivate(q), m_includeRawData(false)
{
}

void FacebookModelBuilderPrivate::properties_append(QQmlListProperty<FacebookProperty> *list, FacebookProperty *property)
{
    FacebookModelBuilder *builder = qobject_cast<FacebookModelBuilder *>(list->object);
    if (!builder || !property) {
        return;
    }
    builder->d_func()->m_properties.append(property);
}

int FacebookModelBuilderPrivate::properties_count(QQmlListProperty<FacebookProperty> *list)
{
    FacebookModelBuilder *builder = qobject_cast<FacebookModelBuilder *>(list->object);
    if (!builder) {
        return 0;
    }
    return builder->d_func()->m_properties.count();
}

FacebookProperty * FacebookModelBuilderPrivate::properties_at(QQmlListProperty<FacebookProperty> *list, int index)
{
    FacebookModelBuilder *builder = qobject_cast<FacebookModelBuilder *>(list->object);
    if (!builder) {
        return nullptr;
    }

    const QList<FacebookProperty *> &properties = builder->d_func()->m_properties;
    if (index < 0 || index >= properties.count()) {
        return nullptr;
    }

    return properties.at(index);
}

void FacebookModelBuilderPrivate::properties_clear(QQmlListProperty<FacebookProperty> *list)
{
    FacebookModelBuilder *builder = qobject_cast<FacebookModelBuilder *>(list->object);
    if (!builder) {
        return;
    }

    builder->d_func()->clear();
}

void FacebookModelBuilderPrivate::setRawData(const QString &rawData)
{
    Q_Q(FacebookModelBuilder);
    if (m_rawData != rawData) {
        m_rawData = rawData;
        emit q->rawDataChanged();
    }
}

void FacebookModelBuilderPrivate::clear()
{
    qDeleteAll(m_properties);
    m_properties.clear();
}

FacebookModelBuilder::FacebookModelBuilder(QObject *parent)
    : SocialContentModelBuilder(*(new FacebookModelBuilderPrivate(this)), parent)
{
}

FacebookModelBuilder::~FacebookModelBuilder()
{
    Q_D(FacebookModelBuilder);
    d->clear();
}

void FacebookModelBuilder::build(SocialContentModel &contentModel,
                                 QNetworkReply::NetworkError error, const QString &errorMessage,
                                 const QByteArray &data, const QVariantMap &metadata)
{
    Q_D(FacebookModelBuilder);
    SocialNetworkError::type outError = SocialNetworkError::No;
    QString outErrorMessage;
    QString outErrorCode;

    QJsonObject root = FacebookPrivate::prebuild(error, errorMessage, data, metadata,
                                                 outError, outErrorMessage, outErrorCode);
    if (outError != SocialNetworkError::No) {
        setError(contentModel, outError, outErrorMessage, outErrorCode);
        return;
    }

    const QString &dataRoot = metadata.value("dataRoot").toString();
    if (!root.contains(dataRoot)) {
        setError(contentModel, SocialNetworkError::Data, "Cannot find data root object");
        return;
    }

    if (d->m_includeRawData) {
        QJsonDocument document = QJsonDocument::fromJson(data);
        d->setRawData(document.toJson(QJsonDocument::Indented));
    }

    const QJsonObject &dataObject = root.value(dataRoot).toObject();
    const QJsonArray &nodes = dataObject.value("nodes").toArray();
    const QJsonObject &pageInfo = dataObject.value("page_info").toObject();

    QList<QVariantMap> returnedData;
    for (const QJsonValue &value : nodes) {
        QVariantMap properties;
        if (d->m_properties.isEmpty()) {
            properties = FacebookPrivate::recursiveValues(value.toObject());
        } else {
            properties = FacebookPrivate::buildProperties(value.toObject(), d->m_properties);
        }
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

QQmlListProperty<FacebookProperty> FacebookModelBuilder::properties()
{
    return QQmlListProperty<FacebookProperty>(this, 0,
                                              &FacebookModelBuilderPrivate::properties_append,
                                              &FacebookModelBuilderPrivate::properties_count,
                                              &FacebookModelBuilderPrivate::properties_at,
                                              &FacebookModelBuilderPrivate::properties_clear);
}

bool FacebookModelBuilder::includeRawData() const
{
    Q_D(const FacebookModelBuilder);
    return d->m_includeRawData;
}

void FacebookModelBuilder::setIncludeRawData(bool includeRawData)
{
    Q_D(FacebookModelBuilder);
    if (d->m_includeRawData != includeRawData) {
        d->m_includeRawData = includeRawData;
        emit includeRawDataChanged();
    }
}

QString FacebookModelBuilder::rawData() const
{
    Q_D(const FacebookModelBuilder);
    return d->m_rawData;
}

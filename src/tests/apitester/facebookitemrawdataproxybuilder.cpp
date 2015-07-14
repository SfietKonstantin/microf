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

#include "facebookitemrawdataproxybuilder.h"
#include <QtCore/QJsonDocument>
#include "facebook/facebookitembuilder.h"
#include "facebook/facebook_p.h"
#include "socialcontentitembuilder_p.h"
#include "proxybuilderhelper.h"

FacebookItemRawDataProxyBuilder::FacebookItemRawDataProxyBuilder(QObject *parent)
    : SocialContentItemBuilder(parent), m_builder(0)
{
}

FacebookItemBuilder * FacebookItemRawDataProxyBuilder::builder() const
{
    return m_builder;
}

void FacebookItemRawDataProxyBuilder::setBuilder(FacebookItemBuilder *builder)
{
    if (m_builder != builder) {
        m_builder = builder;
        emit builderChanged();
    }
}

QString FacebookItemRawDataProxyBuilder::rawData() const
{
    return m_rawData;
}

QString FacebookItemRawDataProxyBuilder::summary() const
{
    return m_summary;
}

void FacebookItemRawDataProxyBuilder::build(SocialContentItem &contentItem,
                                            QNetworkReply::NetworkError error,
                                           const QString &errorMessage, const QByteArray &data,
                                           const QVariantMap &metadata)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    QString rawData = QString::fromLatin1(document.toJson(QJsonDocument::Indented));
    if (m_rawData != rawData) {
        m_rawData = rawData;
        emit rawDataChanged();
    }

    QString summary;
    QTextStream stream (&summary);
    getSummary(document, stream);
    if (m_summary != summary) {
        m_summary = summary;
        emit summaryChanged();
    }

    if (m_builder) {
        SocialContentItemBuilderPrivate::build(*m_builder, contentItem, error, errorMessage,
                                               data, metadata);
    }
}


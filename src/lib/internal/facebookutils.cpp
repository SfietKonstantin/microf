/*
 * Copyright (C) 2016 Lucien XU <sfietkonstantin@free.fr>
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

#include "facebookutils.h"
#include <QJsonDocument>
#include <QUrlQuery>

namespace microf { namespace internal {

FacebookUtils::FacebookUtils(Facebook &facebook)
    : m_facebook {facebook}
{
}

QNetworkRequest FacebookUtils::createHttpRequest(const QByteArray &postData) const
{
    QNetworkRequest request {QUrl("https://graph.facebook.com/graphql")};
    QByteArray authorization {"OAuth "};
    authorization.append(m_facebook.accessToken().toLocal8Bit());
    request.setRawHeader("Authorization", authorization);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Host", "graph.facebook.com");
    request.setRawHeader("Connection", "Keep-Alive");
    request.setRawHeader("Content-Length", QByteArray::number(postData.size()));
    return request;
}

QByteArray FacebookUtils::createPostData(const QByteArray &queryId,
                                         const QByteArray &requestName,
                                         const QJsonObject &object) const
{
    QJsonDocument document {object};
    QUrlQuery query {};
    query.addQueryItem("query_id", queryId);
    query.addQueryItem("query_params", document.toJson(QJsonDocument::Compact));
    query.addQueryItem("fb_api_req_friendly_name", requestName);
    query.addQueryItem("method", "get");
    query.addQueryItem("locale", m_facebook.locale());
    query.addQueryItem("client_country_code", m_facebook.countryCode());
    return query.toString(QUrl::FullyEncoded).toLocal8Bit();
}

}}

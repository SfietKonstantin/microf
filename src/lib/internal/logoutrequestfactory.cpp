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

#include "logoutrequestfactory.h"
#include <QUrlQuery>
#include "icontroller.h"
#include "facebook.h"

namespace microf { namespace internal {

LogoutRequestFactory::LogoutRequestFactory(IController &parent)
    : m_parent(parent)
{
}

std::unique_ptr<LogoutRequestFactory::Job_t> LogoutRequestFactory::create(LogoutRequest &&request) const
{
    return std::unique_ptr<Job_t>(new Job(std::move(request), m_parent));
}

LogoutRequestFactory::Job::Job(LogoutRequest &&request, IController &parent)
    : m_request(std::move(request)), m_parent(parent)
{
}

void LogoutRequestFactory::Job::execute(Job::OnResult_t onResult, Job::OnError_t onError)
{
    Q_UNUSED(onError)
    Q_ASSERT(m_parent.facebook());
    const QByteArray &postData = createPostData();
    QString accessToken {m_request.token()};
    accessToken.prepend("OAuth ");
    QNetworkRequest networkRequest (QUrl("https://b-api.facebook.com/method/auth.expireSession"));
    networkRequest.setRawHeader("Authorization", accessToken.toLocal8Bit());
    networkRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    networkRequest.setRawHeader("Host", "b-api.facebook.com");
    networkRequest.setRawHeader("Connection", "Keep-Alive");
    networkRequest.setRawHeader("User-Agent", m_parent.facebook()->userAgent());
    networkRequest.setRawHeader("Content-Length", QByteArray::number(postData.size()));
    networkRequest.setRawHeader("X-FB-HTTP-Engine", "Apache");
    onResult(HttpRequest(HttpRequest::Type::Post, networkRequest, postData));
}

QByteArray LogoutRequestFactory::Job::createPostData() const
{
    Q_ASSERT(m_parent.facebook());
    QUrlQuery query {};
    query.addQueryItem("reason", "USER_INITIATED");
    query.addQueryItem("locale", m_parent.facebook()->locale());
    query.addQueryItem("client_country_code", m_parent.facebook()->countryCode());
    query.addQueryItem("method", "auth.expireSession");
    query.addQueryItem("fb_api_req_friendly_name", "logout");
    query.addQueryItem("fb_api_caller_class", "com.facebook.katana.server.handler.Fb4aAuthHandler");
    return query.toString(QUrl::FullyEncoded).toLocal8Bit();
}

}}

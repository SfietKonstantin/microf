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

#include "loginrequestfactory.h"
#include <QCryptographicHash>
#include <QUrlQuery>
#include "icontroller.h"
#include "facebook.h"

namespace {

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

}

namespace microf { namespace internal {

LoginRequestFactory::LoginRequestFactory(IController &parent)
    : m_parent(parent)
{
}

std::unique_ptr<LoginRequestFactory::Job_t> LoginRequestFactory::create(LoginRequest &&request) const
{
    return std::unique_ptr<Job_t>(new Job(std::move(request), m_parent));
}

LoginRequestFactory::Job::Job(LoginRequest &&request, IController &parent)
    : m_request(std::move(request)), m_parent(parent)
{
}

void LoginRequestFactory::Job::execute(Job::OnResult_t onResult, Job::OnError_t onError)
{
    Q_UNUSED(onError);
    Q_ASSERT(m_parent.facebook());
    const QByteArray &postData = createPostData();
    QNetworkRequest networkRequest (QUrl("https://b-api.facebook.com/method/auth.login"));
    networkRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    networkRequest.setRawHeader("Host", "b-api.facebook.com");
    networkRequest.setRawHeader("Connection", "Keep-Alive");
    networkRequest.setRawHeader("User-Agent", m_parent.facebook()->userAgent());
    networkRequest.setRawHeader("Content-Length", QByteArray::number(postData.size()));
    networkRequest.setRawHeader("X-FB-HTTP-Engine", "Apache");
    onResult(HttpRequest(HttpRequest::Type::Post, networkRequest, postData));
}

QByteArray LoginRequestFactory::Job::createPostData() const
{
    Q_ASSERT(m_parent.facebook());
    QMap<QString, QString> parameters;
    parameters.insert("api_key", m_parent.facebook()->apiKey());
    parameters.insert("client_country_code", m_parent.facebook()->countryCode());
    parameters.insert("credentials_type", "password");
    parameters.insert("device_id", m_request.deviceId());
    parameters.insert("email", m_request.email());
    parameters.insert("error_detail_type", "button_with_disabled");
    parameters.insert("fb_api_caller_class", "com.facebook.katana.server.handler.Fb4aAuthHandler");
    parameters.insert("fb_api_req_friendly_name", "authenticate");
    parameters.insert("format", "json");
    parameters.insert("generate_session_cookies", "1");
    parameters.insert("locale", m_parent.facebook()->locale());
    if (m_request.machineId().isEmpty()) {
        parameters.insert("generate_machine_id", "1");
    } else {
        parameters.insert("machine_id", m_request.machineId());
    }
    parameters.insert("method", "auth.login");
    parameters.insert("password", m_request.password());

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

    QUrlQuery query {};
    for (const QString &key : parameters.keys()) {
        query.addQueryItem(key, parameters.value(key));
    }
    return query.toString(QUrl::FullyEncoded).toLocal8Bit();
}

}}

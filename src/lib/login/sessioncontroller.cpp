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

#include "sessioncontroller.h"
#include "data/itemmodifier.h"
#include "facebook.h"
#include "core/loginrequest.h"
#include "core/sessionrequestfactory.h"
#include "data/item.h"
#include "http/httprequestfactory.h"
#include "json/jsonrequestfactory.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>

using namespace ::microcore::core;
using namespace ::microcore::error;
using namespace ::microcore::data;
using namespace ::microcore::http;
using namespace ::microcore::json;
using namespace ::microcore::qt;
using namespace ::microcore::fb;

namespace microf { namespace login {

namespace {

static const char *ERROR_DATA_KEY = "error_data";
static const char *ERROR_MESSAGE_KEY = "error_message";
static const char *UID_KEY = "uid";

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

struct Empty
{
    using SourceItem_t = Empty;
    void setData(const Empty &data)
    {
        Q_UNUSED(data)
    }
};

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

using SessionItem_t = Item<Session>;
class LoginModifier: public ItemModifier<SessionItem_t, LoginRequest, Error>
{
public:
    explicit LoginModifier(SessionItem_t &item,
                           SessionController &parent,
                           QNetworkAccessManager &network)
        : ItemModifier<SessionItem_t, LoginRequest, Error>(item, std::unique_ptr<Factory_t>(new JobFactory(parent, network)))
    {
    }
private:
    class LoginRequestFactory: public IJobFactory<LoginRequest, HttpRequest, Error>
    {
    public:
        using Job_t = IJob<HttpRequest, Error>;
        explicit LoginRequestFactory(SessionController &parent)
            : m_parent(parent)
        {
        }
        std::unique_ptr<Job_t> create(LoginRequest &&request) const override
        {
            return std::unique_ptr<Job_t>(new Job(std::move(request), m_parent));
        }
    private:
        class Job : public Job_t
        {
        public:
            explicit Job(LoginRequest &&request, SessionController &parent)
                : m_request(std::move(request)), m_parent(parent)
            {
            }
            void execute(OnResult_t onResult, OnError_t onError) override
            {
                Q_UNUSED(onError);
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
        private:
            QByteArray createPostData() const
            {
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

                QUrlQuery query;
                for (const QString &key : parameters.keys()) {
                    query.addQueryItem(key, parameters.value(key));
                }
                return query.toString(QUrl::FullyEncoded).toLocal8Bit();
            }
            LoginRequest m_request {};
            SessionController &m_parent;
        };
        SessionController &m_parent;
    };
    class SessionPreprocessFactory: public IJobFactory<JsonResult, JsonResult, Error>
    {
    public:
        using Job_t = IJob<JsonResult, Error>;
        std::unique_ptr<Job_t> create(JsonResult &&request) const override
        {
            return std::unique_ptr<Job_t>(new Job(std::move(request)));
        }
    private:
        class Job: public Job_t
        {
        public:
            explicit Job(JsonResult &&json)
                : m_json(std::move(json))
            {
            }
            void execute(OnResult_t onResult, OnError_t onError) override
            {
                QJsonObject object {m_json.object()};
                // Check if we got an error
                if (object.contains(QLatin1String(ERROR_DATA_KEY))) {
                    QString errorData {object.value(QLatin1String(ERROR_DATA_KEY)).toString()};
                    QJsonDocument errorDocument {QJsonDocument::fromJson(errorData.toLocal8Bit())};
                    if (!errorDocument.isObject()) {
                        onError(Error("fb-session-pre",
                                      QLatin1String("Cannot convert error message to JSON"),
                                      errorData.toLocal8Bit()));
                        return;
                    }

                    const QJsonObject &errorObject {errorDocument.object()};
                    if (!errorObject.contains(QLatin1String(ERROR_MESSAGE_KEY))) {
                        onError(Error("fb-session-pre",
                                      QLatin1String("Cannot convert error title in error message"),
                                      errorData.toLocal8Bit()));
                        return;
                    }
                    QString errorTitle = errorObject.value(QLatin1String(ERROR_MESSAGE_KEY)).toString();
                    onError(Error("fb-session-pre", errorTitle, QByteArray()));
                    return;
                }

                const QJsonValue &uid {object.value(QLatin1String(UID_KEY))};
                if (!uid.isDouble() && !uid.isString()) {
                    object.remove(QLatin1String(UID_KEY));
                } else {
                    if (uid.isDouble()) {
                        object.insert(QLatin1String(UID_KEY), QString::number(uid.toVariant().toInt()));
                    }
                }
                onResult(QJsonDocument(object));
            }
        private:
            JsonResult m_json {};
        };
    };
    class JobFactory: public IJobFactory<LoginRequest, Session, Error>
    {
    public:
        using Job_t = IJob<Session, Error>;
        explicit JobFactory(SessionController &parent,
                            QNetworkAccessManager &network)
            : m_loginFactory(parent)
            , m_httpFactory (network)
        {
        }
        std::unique_ptr<Job_t> create(LoginRequest &&request) const override
        {
            return std::unique_ptr<Job_t>(new Job(std::move(request), *this));
        }
    private:
        class Job: public Job_t
        {
        public:
            explicit Job(LoginRequest &&request, const JobFactory &parent)
                : m_request(std::move(request)), m_parent(parent)
            {
            }
            void execute(OnResult_t onResult, OnError_t onError) override
            {
                m_preToSession.reset(new Pipe<JsonResult, Session, Error>(m_parent.m_sessionFactory, onResult, onError));
                m_jsonToPre = std::move(m_preToSession->prepend(m_parent.m_sessionPreFactory));
                m_httpToJson = std::move(m_jsonToPre->prepend(m_parent.m_jsonFactory));
                m_httpRequestToResult = std::move(m_httpToJson->prepend(m_parent.m_httpFactory));
                m_requestToHttpRequest = std::move(m_httpRequestToResult->prepend(m_parent.m_loginFactory));
                m_requestToHttpRequest->send(std::move(m_request));
            }
        private:
            LoginRequest m_request {};
            const JobFactory &m_parent;
            std::unique_ptr<Pipe<LoginRequest, HttpRequest, Error>> m_requestToHttpRequest {};
            std::unique_ptr<Pipe<HttpRequest, HttpResult, Error>> m_httpRequestToResult {};
            std::unique_ptr<Pipe<HttpResult, JsonResult, Error>> m_httpToJson {};
            std::unique_ptr<Pipe<JsonResult, JsonResult, Error>> m_jsonToPre {};
            std::unique_ptr<Pipe<JsonResult, Session, Error>> m_preToSession {};
        };
        LoginRequestFactory m_loginFactory;
        HttpRequestFactory m_httpFactory;
        JsonRequestFactory m_jsonFactory {};
        SessionPreprocessFactory m_sessionPreFactory {};
        SessionRequestFactory m_sessionFactory {};
    };
};

class LogoutRequest
{
public:
    explicit LogoutRequest() = default;
    explicit LogoutRequest(QString &&token) : m_token (token) {}
    DEFAULT_COPY_DEFAULT_MOVE(LogoutRequest);
    QString token() const { return m_token; }
private:
    QString m_token {};
};

class LogoutModifier: public ItemModifier<Empty, LogoutRequest, Error>
{
public:
    explicit LogoutModifier(SessionController &parent,
                            QNetworkAccessManager &network)
        : ItemModifier<Empty, LogoutRequest, Error>(m_item, std::unique_ptr<Factory_t>(new JobFactory(parent, network)))
    {
    }
private:
    class LogoutRequestFactory: public IJobFactory<LogoutRequest, HttpRequest, Error>
    {
    public:
        using Job_t = IJob<HttpRequest, Error>;
        explicit LogoutRequestFactory(SessionController &parent)
            : m_parent(parent)
        {
        }
        std::unique_ptr<Job_t> create(LogoutRequest &&request) const override
        {
            return std::unique_ptr<Job_t>(new Job(std::move(request), m_parent));
        }
    private:
        class Job : public Job_t
        {
        public:
            explicit Job(LogoutRequest &&request, SessionController &parent)
                : m_request(std::move(request)), m_parent(parent)
            {
            }
            void execute(OnResult_t onResult, OnError_t onError) override
            {
                Q_UNUSED(onError)
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
        private:
            QByteArray createPostData() const
            {
                QUrlQuery query;
                query.addQueryItem("reason", "USER_INITIATED");
                query.addQueryItem("locale", m_parent.facebook()->locale());
                query.addQueryItem("client_country_code", m_parent.facebook()->countryCode());
                query.addQueryItem("method", "auth.expireSession");
                query.addQueryItem("fb_api_req_friendly_name", "logout");
                query.addQueryItem("fb_api_caller_class", "com.facebook.katana.server.handler.Fb4aAuthHandler");
                return query.toString(QUrl::FullyEncoded).toLocal8Bit();
            }
            LogoutRequest m_request {};
            SessionController &m_parent;
        };
        SessionController &m_parent;
    };
    class EmptyFactory: public IJobFactory<HttpResult, Empty, Error>
    {
    public:
        using Job_t = IJob<Empty, Error>;
        explicit EmptyFactory()

        {
        }
        std::unique_ptr<Job_t> create(HttpResult &&request) const override
        {
            return std::unique_ptr<Job_t>(new Job(std::move(request)));
        }
    private:
        class Job: public Job_t
        {
        public:
            explicit Job(HttpResult &&request)
                : m_request(std::move(request))
            {
            }
            void execute(OnResult_t onResult, OnError_t onError) override
            {
                Q_UNUSED(onError)
                onResult(Empty());
            }
        private:
            HttpResult m_request {};
        };
    };
    class JobFactory: public IJobFactory<LogoutRequest, Empty, Error>
    {
    public:
        using Job_t = IJob<Empty, Error>;
        explicit JobFactory(SessionController &parent,
                            QNetworkAccessManager &network)
            : m_logoutFactory(parent)
            , m_httpFactory (network)
        {
        }
        std::unique_ptr<Job_t> create(LogoutRequest &&request) const override
        {
            return std::unique_ptr<Job_t>(new Job(std::move(request), *this));
        }
    private:
        class Job: public Job_t
        {
        public:
            explicit Job(LogoutRequest &&request, const JobFactory &parent)
                : m_request(std::move(request)), m_parent(parent)
            {
            }
            void execute(OnResult_t onResult, OnError_t onError) override
            {
                m_httpToEmpty.reset(new Pipe<HttpResult, Empty, Error>(m_parent.m_emptyFactory, onResult, onError));
                m_httpRequestToResult = std::move(m_httpToEmpty->prepend(m_parent.m_httpFactory));
                m_requestToHttpRequest = std::move(m_httpRequestToResult->prepend(m_parent.m_logoutFactory));
                m_requestToHttpRequest->send(std::move(m_request));
            }
        private:
            LogoutRequest m_request {};
            const JobFactory &m_parent;
            std::unique_ptr<Pipe<LogoutRequest, HttpRequest, Error>> m_requestToHttpRequest {};
            std::unique_ptr<Pipe<HttpRequest, HttpResult, Error>> m_httpRequestToResult {};
            std::unique_ptr<Pipe<HttpResult, Empty, Error>> m_httpToEmpty {};
        };
        LogoutRequestFactory m_logoutFactory;
        HttpRequestFactory m_httpFactory;
        EmptyFactory m_emptyFactory {};
    };
    Empty m_item {};
};

}

SessionController::SessionController(QObject *parent)
    : ViewItemController<SessionItem_t>(parent)
{
}

SessionItem_t & SessionController::item()
{
    return m_data;
}

::microf::Facebook * SessionController::facebook() const
{
    return m_facebook;
}

void SessionController::setFacebook(::microf::Facebook *facebook)
{
    if (m_facebook != facebook) {
        m_facebook = facebook;
        Q_EMIT facebookChanged();
    }
}

QString SessionController::email() const
{
    return m_email;
}

void SessionController::setEmail(const QString &email)
{
    if (m_email != email) {
        m_email = email;
        Q_EMIT emailChanged();
    }
}

QString SessionController::password() const
{
    return m_password;
}

void SessionController::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        Q_EMIT passwordChanged();
    }
}

QString SessionController::deviceId() const
{
    return m_deviceId;
}

void SessionController::setDeviceId(const QString &deviceId)
{
    if (m_deviceId != deviceId) {
        m_deviceId = deviceId;
        Q_EMIT deviceIdChanged();
    }
}

QString SessionController::machineId() const
{
    return m_machineId;
}

void SessionController::setMachineId(const QString &machineId)
{
    if (m_machineId != machineId) {
        m_machineId = machineId;
        Q_EMIT machineIdChanged();
    }
}

bool SessionController::login()
{
    if (m_facebook == nullptr) {
        return false;
    }
    registerExecutors();

    static_cast<LoginModifier *>(m_loginExecutor)->start(LoginRequest(QString(m_email),
                                                                      QString(m_password),
                                                                      QString(m_deviceId),
                                                                      QString(m_machineId)));
    return true;
}

bool SessionController::logout(const QString &accessToken)
{
    if (m_facebook == nullptr) {
        return false;
    }
    if (accessToken.isEmpty()) {
        return false;
    }
    registerExecutors();

    static_cast<LogoutModifier *>(m_logoutExecutor)->start(LogoutRequest(QString(accessToken)));
    return true;
}

void SessionController::registerExecutors()
{
    if (m_loginExecutor != nullptr && m_logoutExecutor != nullptr) {
        return;
    }

    QQmlContext *context {QQmlEngine::contextForObject(this)};
    if (context == nullptr) {
        return;
    }
    QNetworkAccessManager *network {context->engine()->networkAccessManager()};
    Q_ASSERT(network);

    if (m_loginExecutor == nullptr) {
        m_loginExecutor = &(addExecutor(std::unique_ptr<LoginModifier>(new LoginModifier(m_data, *this, *network))));
    }

    if (m_logoutExecutor == nullptr) {
        m_logoutExecutor = &(addExecutor(std::unique_ptr<LogoutModifier>(new LogoutModifier(*this, *network))));
    }
}

}}

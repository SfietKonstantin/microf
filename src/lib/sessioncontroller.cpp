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
#include "data/item.h"
#include "core/sessionrequestfactory.h"
#include "http/httprequestfactory.h"
#include "json/jsonrequestfactory.h"
#include "internal/emptyresultfactory.h"
#include "internal/loginrequestfactory.h"
#include "internal/loginresultpreprocessfactory.h"
#include "internal/logoutrequestfactory.h"
#include <QQmlContext>
#include <QQmlEngine>

using namespace ::microcore::core;
using namespace ::microcore::error;
using namespace ::microcore::data;
using namespace ::microcore::http;
using namespace ::microcore::json;
using namespace ::microcore::qt;
using namespace ::microcore::fb;
using namespace ::microf::internal;

namespace microf {

namespace {

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
        LoginResultPreprocessFactory m_sessionPreFactory {};
        SessionRequestFactory m_sessionFactory {};
    };
};

class LogoutModifier: public ItemModifier<EmptyResult, LogoutRequest, Error>
{
public:
    explicit LogoutModifier(SessionController &parent,
                            QNetworkAccessManager &network)
        : ItemModifier<EmptyResult, LogoutRequest, Error>(m_item, std::unique_ptr<Factory_t>(new JobFactory(parent, network)))
    {
    }
private:
    class JobFactory: public IJobFactory<LogoutRequest, EmptyResult, Error>
    {
    public:
        using Job_t = IJob<EmptyResult, Error>;
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
                m_httpToEmpty.reset(new Pipe<HttpResult, EmptyResult, Error>(m_parent.m_emptyFactory, onResult, onError));
                m_httpRequestToResult = std::move(m_httpToEmpty->prepend(m_parent.m_httpFactory));
                m_requestToHttpRequest = std::move(m_httpRequestToResult->prepend(m_parent.m_logoutFactory));
                m_requestToHttpRequest->send(std::move(m_request));
            }
        private:
            LogoutRequest m_request {};
            const JobFactory &m_parent;
            std::unique_ptr<Pipe<LogoutRequest, HttpRequest, Error>> m_requestToHttpRequest {};
            std::unique_ptr<Pipe<HttpRequest, HttpResult, Error>> m_httpRequestToResult {};
            std::unique_ptr<Pipe<HttpResult, EmptyResult, Error>> m_httpToEmpty {};
        };
        LogoutRequestFactory m_logoutFactory;
        HttpRequestFactory m_httpFactory;
        EmptyResultFactory m_emptyFactory {};
    };
    EmptyResult m_item {};
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

    LoginModifier *loginModifier {static_cast<LoginModifier *>(m_loginExecutor)};
    loginModifier->start(LoginRequest(QString(m_email), QString(m_password),
                                      QString(m_deviceId), QString(m_machineId)));
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

}

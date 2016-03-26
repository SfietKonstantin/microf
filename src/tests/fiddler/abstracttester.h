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

#ifndef ABSTRACTTESTER_H
#define ABSTRACTTESTER_H

#include <QObject>
#include <QQmlParserStatus>
#include "icontroller.h"
#include "core/pipe.h"
#include "internal/threadsrequestfactory.h"
#include "http/httprequestfactory.h"
#include "json/jsonrequestfactory.h"
#include "jsontreemodel.h"

using ::microf::internal::EmptyRequest;
using ::microf::internal::ThreadsRequestFactory;
using ::microcore::core::Pipe;
using ::microcore::core::IJobFactory;
using ::microcore::http::HttpRequest;
using ::microcore::http::HttpResult;
using ::microcore::http::HttpRequestFactory;
using ::microcore::error::Error;
using ::microcore::json::JsonRequestFactory;
using ::microcore::json::JsonResult;

class AbstractTester : public QObject , public QQmlParserStatus , public ::microf::IController
{
public:
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(microf::Facebook * facebook READ facebook WRITE setFacebook NOTIFY facebookChanged)
    Q_PROPERTY(JsonTreeModel * model READ model WRITE setModel NOTIFY modelChanged)
public:
    void classBegin() override;
    void componentComplete() override;
    ::microf::Facebook * facebook() const override;
    void setFacebook(::microf::Facebook *facebook);
    JsonTreeModel * model() const;
    void setModel(JsonTreeModel *model);
public slots:
    virtual void performQuery() = 0;
signals:
    void facebookChanged();
    void modelChanged();
protected:
    explicit AbstractTester(QObject *parent = nullptr);
    template<class Request> std::unique_ptr<Pipe<Request, HttpRequest, Error>> buildPipes(const IJobFactory<Request, HttpRequest, Error> &factory)
    {
        std::function<void (JsonResult &&)> onResultFunction = std::bind(&AbstractTester::onResult, this, std::placeholders::_1);
        m_httpResultToJson.reset(new Pipe<HttpResult, JsonResult, Error>(m_jsonRequestFactory,
                                                                          onResultFunction, onError));
        m_httpRequestToResult = std::move(m_httpResultToJson->prepend(*m_httpRequestFactory));
        return m_httpRequestToResult->prepend(factory);
    }
private:
    void onResult(JsonResult &&result);
    static void onError(Error &&error);
    ::microf::Facebook *m_facebook {nullptr};
    JsonTreeModel *m_model {nullptr};
    std::unique_ptr<HttpRequestFactory> m_httpRequestFactory {};
    JsonRequestFactory m_jsonRequestFactory {};
    std::unique_ptr<Pipe<HttpRequest, HttpResult, Error>> m_httpRequestToResult {};
    std::unique_ptr<Pipe<HttpResult, JsonResult, Error>> m_httpResultToJson {};
};

#endif // ABSTRACTTESTER_H

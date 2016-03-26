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

#include "abstracttester.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonObject>

using namespace ::microf;

AbstractTester::AbstractTester(QObject *parent)
    : QObject(parent)
{
}

void AbstractTester::classBegin()
{
}

void AbstractTester::componentComplete()
{
    QQmlContext *context {QQmlEngine::contextForObject(this)};
    Q_ASSERT(context);
    QNetworkAccessManager *network {context->engine()->networkAccessManager()};
    Q_ASSERT(network);
    m_httpRequestFactory.reset(new HttpRequestFactory(*network));
}

Facebook * AbstractTester::facebook() const
{
    return m_facebook;
}

void AbstractTester::setFacebook(Facebook *facebook)
{
    if (m_facebook != facebook) {
        m_facebook = facebook;
        emit facebookChanged();
    }
}

JsonTreeModel * AbstractTester::model() const
{
    return m_model;
}

void AbstractTester::setModel(JsonTreeModel *model)
{
    if (m_model != model) {
        m_model = model;
        emit modelChanged();
    }
}

void AbstractTester::onResult(microcore::json::JsonResult &&result)
{
    if (m_model == nullptr) {
        return;
    }

    QJsonValue value {};
    if (result.isObject()) {
        value = QJsonObject(result.object());
    } else if (result.isArray()) {
        value = QJsonArray(result.array());
    }
    m_model->setValue(std::move(value));
}

void AbstractTester::onError(microcore::error::Error &&error)
{
    Q_UNUSED(error)
}

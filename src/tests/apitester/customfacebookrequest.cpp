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

#include "customfacebookrequest.h"
#include "socialrequest_p.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

class CustomFacebookRequestPrivate: public SocialRequestPrivate
{
public:
    explicit CustomFacebookRequestPrivate(CustomFacebookRequest *q);
private:
    Q_DECLARE_PUBLIC(CustomFacebookRequest)
    QString m_queryId;
    QString m_requestName;
    QString m_apiCallerClass;
    QString m_queryParametersJson;
};

CustomFacebookRequestPrivate::CustomFacebookRequestPrivate(CustomFacebookRequest *q)
    : SocialRequestPrivate(q)
{
}

CustomFacebookRequest::CustomFacebookRequest(QObject *parent)
    : AbstractFacebookRequest(*(new CustomFacebookRequestPrivate(this)), parent)
{
}

QString CustomFacebookRequest::queryId() const
{
    Q_D(const CustomFacebookRequest);
    return d->m_queryId;
}

void CustomFacebookRequest::setQueryId(const QString &queryId)
{
    Q_D(CustomFacebookRequest);
    if (d->m_queryId != queryId) {
        d->m_queryId = queryId;
        emit queryIdChanged();
    }
}

QString CustomFacebookRequest::requestName() const
{
    Q_D(const CustomFacebookRequest);
    return d->m_requestName;
}

void CustomFacebookRequest::setRequestName(const QString &requestName)
{
    Q_D(CustomFacebookRequest);
    if (d->m_requestName != requestName) {
        d->m_requestName = requestName;
        emit requestNameChanged();
    }
}

QString CustomFacebookRequest::apiCallerClass() const
{
    Q_D(const CustomFacebookRequest);
    return d->m_apiCallerClass;
}

void CustomFacebookRequest::setApiCallerClass(const QString &apiCallerClass)
{
    Q_D(CustomFacebookRequest);
    if (d->m_apiCallerClass != apiCallerClass) {
        d->m_apiCallerClass = apiCallerClass;
        emit apiCallerClassChanged();
    }
}

QString CustomFacebookRequest::queryParametersJson() const
{
    Q_D(const CustomFacebookRequest);
    return d->m_queryParametersJson;
}

void CustomFacebookRequest::setQueryParametersJson(const QString &queryParametersJson)
{
    Q_D(CustomFacebookRequest);
    if (d->m_queryParametersJson != queryParametersJson) {
        d->m_queryParametersJson = queryParametersJson;
        emit queryParametersJsonChanged();
    }
}

QJsonObject CustomFacebookRequest::queryParameters(const Facebook &facebook, SocialRequest::Mode mode,
                                                   const QVariantMap &metadata) const
{
    Q_D(const CustomFacebookRequest);
    Q_UNUSED(facebook)
    Q_UNUSED(mode)
    Q_UNUSED(metadata)
    QJsonDocument document = QJsonDocument::fromJson(d->m_queryParametersJson.toLocal8Bit());
    return document.object();
}


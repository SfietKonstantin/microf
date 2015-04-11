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

#include "socialnetwork.h"
#include "socialnetwork_p.h"
#include "socialrequest.h"
#include "socialrequest_p.h"
#include "socialcontentitem_p.h"
#include <QtCore/QDebug>

SocialNetworkPrivate::SocialNetworkPrivate(SocialNetwork *q)
    : q_ptr(q), m_networkAccess(0)
{
}

bool SocialNetworkPrivate::contentItemLoad(SocialNetwork &socialNetwork, SocialContentItem &contentItem,
                                const SocialRequest &request)
{
    return socialNetwork.d_func()->contentItemLoad(contentItem, request);
}

bool SocialNetworkPrivate::contentItemLoad(SocialContentItem &contentItem, const SocialRequest &request)
{
    Q_Q(SocialNetwork);
    if (m_replies.contains(&contentItem)) {
        qDebug() << "SocialNetworkPrivate::load() called when SocialContentItem is already loading";
        return false;
    }

    QByteArray postData;
    SocialRequest::Type type = request.type();
    if (type == SocialRequest::Post) {
        postData = SocialRequestPrivate::createPostData(request, *q);
    }
    QNetworkRequest networkRequest = SocialRequestPrivate::createRequest(request, *q, postData);
    QNetworkReply *reply = 0;
    switch (type) {
    case SocialRequest::Get:
        reply = m_networkAccess->get(networkRequest);
        break;
    case SocialRequest::Post:
        reply = m_networkAccess->post(networkRequest, postData);
        break;
    case SocialRequest::Delete:
        reply = m_networkAccess->deleteResource(networkRequest);
        break;
    }

    if (!reply) {
        qDebug() << "SocialNetworkPrivate::load() cannot perform request, check request type";
        return false;
    }

    m_replies.insert(&contentItem);
    QObject::connect(reply, &QNetworkReply::finished, [this, &contentItem, reply]{
        SocialContentItemPrivate::handleNetworkReply(contentItem, reply->error(),
                                                     reply->errorString(), reply->readAll());
        reply->deleteLater();
        m_replies.remove(&contentItem);
    });

    return true;
}

SocialNetwork::SocialNetwork(QObject *parent)
    : QObject(parent), d_ptr(new SocialNetworkPrivate(this))
{
    Q_D(SocialNetwork);
    d->m_networkAccess = new QNetworkAccessManager(this);
}

SocialNetwork::SocialNetwork(SocialNetworkPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(SocialNetwork);
    d->m_networkAccess = new QNetworkAccessManager(this);
}

SocialNetwork::~SocialNetwork()
{
}

void SocialNetwork::classBegin()
{
}

void SocialNetwork::componentComplete()
{
}


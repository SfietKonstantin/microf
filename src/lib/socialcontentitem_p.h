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

#ifndef SOCIALCONTENTITEM_P_H
#define SOCIALCONTENTITEM_P_H

#include <QtNetwork/QNetworkReply>
#include "socialnetworkstatus.h"
#include "socialnetworkerror.h"

class SocialNetwork;
class SocialObject;
class SocialRequest;
class SocialContentItemBuilder;
class SocialContentItem;
class SocialContentItemPrivate
{
public:
    explicit SocialContentItemPrivate(SocialContentItem *q);
    static void handleNetworkReply(SocialContentItem &contentItem,
                                   QNetworkReply::NetworkError error, const QString &errorString,
                                   const QByteArray &data);
    static void setContentItemObject(SocialContentItem &contentItem, const QVariantMap &properties);
    static void setContentItemError(SocialContentItem &contentItem,
                                    SocialNetworkError::type error, const QString &errorString);
protected:
    SocialContentItem * const q_ptr;
private:
    void handleNetworkReply(QNetworkReply::NetworkError error, const QString &errorString,
                            const QByteArray &data);
    void setContentItemObject(const QVariantMap &properties);
    void setStatus(SocialNetworkStatus::type status);
    void setError(SocialNetworkError::type error, const QString &errorString);
    SocialNetwork *m_socialNetwork;
    SocialRequest *m_request;
    SocialContentItemBuilder *m_builder;
    SocialObject *m_object;
    SocialNetworkStatus::type m_status;
    SocialNetworkError::type m_error;
    QString m_errorString;
    Q_DECLARE_PUBLIC(SocialContentItem)
};

#endif // SOCIALCONTENTITEM_P_H


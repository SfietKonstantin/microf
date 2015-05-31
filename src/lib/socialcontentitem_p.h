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

#include "abstractsocialcontent_p.h"
#include "socialcontentitem.h"

class SocialContentItemPrivate: public AbstractSocialContentPrivate
{
public:
    explicit SocialContentItemPrivate(SocialContentItem *q);
    static void setContentItemObject(SocialContentItem &contentItem, const QVariantMap &properties,
                                     const QVariantMap &metadata);
    static void setContentItemError(SocialContentItem &contentItem,
                                    SocialNetworkError::type error, const QString &errorMessage,
                                    const QString &errorCode);
protected:
    bool build(QNetworkReply::NetworkError error, const QString &errorMessage,
               const QByteArray &data) Q_DECL_OVERRIDE;
private:
    void setContentItemObject(const QVariantMap &object, const QVariantMap &metadata);
    void setObject(QVariantMap object);
    SocialNetwork *m_socialNetwork;
    SocialRequest *m_request;
    SocialContentItemBuilder *m_builder;
    QVariantMap m_object;
    Q_DECLARE_PUBLIC(SocialContentItem)
};

#endif // SOCIALCONTENTITEM_P_H


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

#include "socialcontentitembuilder.h"
#include "socialcontentitembuilder_p.h"
#include "socialcontentitem_p.h"
#ifdef MICROF_DEBUG
#include <QtCore/QDebug>
#endif

SocialContentItemBuilderPrivate::SocialContentItemBuilderPrivate(SocialContentItemBuilder *q)
    : q_ptr(q)
{
}

void SocialContentItemBuilderPrivate::build(SocialContentItemBuilder &builder,
                                            SocialContentItem &contentItem,
                                            QNetworkReply::NetworkError error,
                                            const QString &errorMessage, const QByteArray &data,
                                            const QVariantMap &metadata)
{
#ifdef MICROF_DEBUG
    qDebug() << "SocialContentItemBuilderPrivate::build";
    qDebug() << "Network error:" << error;
    qDebug() << "Error message:" << errorMessage;
    qDebug() << "Data:" << data;
    qDebug() << "Metadata" << metadata;
#endif
    builder.build(contentItem, error, errorMessage, data, metadata);
}

SocialContentItemBuilder::SocialContentItemBuilder(QObject *parent)
    : QObject(parent), d_ptr(new SocialContentItemBuilderPrivate(this))
{
}

SocialContentItemBuilder::SocialContentItemBuilder(SocialContentItemBuilderPrivate &dd,
                                                   QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

SocialContentItemBuilder::~SocialContentItemBuilder()
{
}

void SocialContentItemBuilder::classBegin()
{
}

void SocialContentItemBuilder::componentComplete()
{
}

void SocialContentItemBuilder::setObject(SocialContentItem &contentItem,
                                         const QVariantMap &properties,
                                         const QVariantMap &metadata)
{
#ifdef MICROF_DEBUG
    qDebug() << "SocialContentItemBuilder::setObject";
    qDebug() << "Properties:" << properties;
    qDebug() << "Metadata" << metadata;
#endif
    SocialContentItemPrivate::setContentItemObject(contentItem, properties, metadata);
}

void SocialContentItemBuilder::setError(SocialContentItem &contentItem,
                                        SocialNetworkError::type error, const QString &errorMessage,
                                        const QString &errorCode)
{
#ifdef MICROF_DEBUG
    qDebug() << "SocialContentItemBuilder::setError";
    qDebug() << "Error:" << error;
    qDebug() << "Error message" << errorMessage;
    qDebug() << "Error code" << errorCode;
#endif
    SocialContentItemPrivate::setContentItemError(contentItem, error, errorMessage, errorCode);
}




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

#ifndef CUSTOMFACEBOOKREQUEST_H
#define CUSTOMFACEBOOKREQUEST_H

#include "facebook/abstractfacebookrequest.h"

class CustomFacebookRequestPrivate;
class CustomFacebookRequest : public AbstractFacebookRequest
{
    Q_OBJECT
    Q_PROPERTY(QString queryId READ queryId WRITE setQueryId NOTIFY queryIdChanged)
    Q_PROPERTY(QString requestName READ requestName WRITE setRequestName NOTIFY requestNameChanged)
    Q_PROPERTY(QString apiCallerClass READ apiCallerClass WRITE setApiCallerClass
               NOTIFY apiCallerClassChanged)
    Q_PROPERTY(QString queryParametersJson READ queryParametersJson WRITE setQueryParametersJson
               NOTIFY queryParametersJsonChanged)
public:
    explicit CustomFacebookRequest(QObject *parent = 0);
    QString queryId() const override;
    void setQueryId(const QString &queryId);
    QString requestName() const override;
    void setRequestName(const QString &requestName);
    QString apiCallerClass() const override;
    void setApiCallerClass(const QString &apiCallerClass);
    QString queryParametersJson() const;
    void setQueryParametersJson(const QString &queryParametersJson);
signals:
    void queryIdChanged();
    void requestNameChanged();
    void apiCallerClassChanged();
    void queryParametersJsonChanged();

protected:
    QJsonObject queryParameters(Mode mode, const QVariantMap &metadata) const override;
private:
    Q_DECLARE_PRIVATE(CustomFacebookRequest)
};

#endif // CUSTOMFACEBOOKREQUEST_H

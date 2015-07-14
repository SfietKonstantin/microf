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

#include "customfacebookmodelbuilder.h"
#include <facebook/abstractfacebookmodelbuilder_p.h>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <facebook/facebook_p.h>

class CustomFacebookModelBuilderPrivate: public AbstractFacebookModelBuilderPrivate
{
public:
    explicit CustomFacebookModelBuilderPrivate(CustomFacebookModelBuilder *q);
    QString path;
};

CustomFacebookModelBuilderPrivate::CustomFacebookModelBuilderPrivate(CustomFacebookModelBuilder *q)
    : AbstractFacebookModelBuilderPrivate(q)
{
}

CustomFacebookModelBuilder::CustomFacebookModelBuilder(QObject *parent)
    : AbstractFacebookModelBuilder(*(new CustomFacebookModelBuilderPrivate(this)), parent)
{
}

QString CustomFacebookModelBuilder::path() const
{
    Q_D(const CustomFacebookModelBuilder);
    return d->path;
}

void CustomFacebookModelBuilder::setPath(const QString &path)
{
    Q_D(CustomFacebookModelBuilder);
    if (d->path != path) {
        d->path = path;
        emit pathChanged();
    }
}

void CustomFacebookModelBuilder::build(SocialContentModel &contentModel,
                                       QNetworkReply::NetworkError error,
                                       const QString &errorMessage, const QByteArray &data,
                                       const QVariantMap &metadata)
{
    Q_D(CustomFacebookModelBuilder);
    Q_UNUSED(metadata)
    SocialNetworkError::type outError = SocialNetworkError::None;
    QString outErrorMessage;
    QString outErrorCode;

    const QJsonObject &root = FacebookPrivate::checkError(error, errorMessage, data, outError,
                                                          outErrorMessage, outErrorCode);
    if (outError != SocialNetworkError::None) {
        setError(contentModel, outError, outErrorMessage, outErrorCode);
        return;
    }

    QJsonObject object = root;
    QStringList splittedPath = d->path.split("/");
    if (splittedPath.isEmpty()) {
        setError(contentModel, SocialNetworkError::Data, "Wrong path");
        return;
    }

    QString last = splittedPath.takeLast().trimmed();
    for (const QString &entry : splittedPath) {
        const QJsonValue &objectValue = object.value(entry.trimmed());
        if (!objectValue.isObject()) {
            setError(contentModel, SocialNetworkError::Data, "Wrong path");
            return;
        }
        object = objectValue.toObject();
    }

    const QJsonValue &arrayValue = object.value(last);
    if (!arrayValue.isArray()) {
        setError(contentModel, SocialNetworkError::Data, "Wrong path");
        return;
    }

    const QJsonArray &array = arrayValue.toArray();
    QList<QVariantMap> returnedData;
    for (const QJsonValue &value : array) {
        returnedData.append(FacebookPrivate::buildProperties(value.toObject(), d->properties()));
    }

    setData(contentModel, returnedData, false, false);
}



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

#include "loginresultpreprocessfactory.h"
#include <QJsonObject>
#include <QVariant>

namespace {

static const char *ERROR_DATA_KEY = "error_data";
static const char *ERROR_MESSAGE_KEY = "error_message";
static const char *UID_KEY = "uid";

}

namespace microf { namespace internal {

std::unique_ptr<LoginResultPreprocessFactory::Job_t> LoginResultPreprocessFactory::create(JsonResult &&request) const
{
    return std::unique_ptr<Job_t>(new Job(std::move(request)));
}

LoginResultPreprocessFactory::Job::Job(JsonResult &&json)
    : m_json(std::move(json))
{
}

void LoginResultPreprocessFactory::Job::execute(Job::OnResult_t onResult, Job::OnError_t onError)
{
    QJsonObject object {m_json.object()};
    // Check if we got an error
    if (object.contains(QLatin1String(ERROR_DATA_KEY))) {
        QString errorData {object.value(QLatin1String(ERROR_DATA_KEY)).toString()};
        QJsonDocument errorDocument {QJsonDocument::fromJson(errorData.toLocal8Bit())};
        if (!errorDocument.isObject()) {
            onError(Error("fb-session-pre",
                          QLatin1String("Cannot convert error message to JSON"),
                          errorData.toLocal8Bit()));
            return;
        }

        const QJsonObject &errorObject {errorDocument.object()};
        if (!errorObject.contains(QLatin1String(ERROR_MESSAGE_KEY))) {
            onError(Error("fb-session-pre",
                          QLatin1String("Cannot convert error title in error message"),
                          errorData.toLocal8Bit()));
            return;
        }
        QString errorTitle = errorObject.value(QLatin1String(ERROR_MESSAGE_KEY)).toString();
        onError(Error("fb-session-pre", errorTitle, QByteArray()));
        return;
    }

    const QJsonValue &uid {object.value(QLatin1String(UID_KEY))};
    if (!uid.isDouble() && !uid.isString()) {
        object.remove(QLatin1String(UID_KEY));
    } else {
        if (uid.isDouble()) {
            object.insert(QLatin1String(UID_KEY), QString::number(uid.toVariant().toInt()));
        }
    }
    onResult(QJsonDocument(object));
}

}}

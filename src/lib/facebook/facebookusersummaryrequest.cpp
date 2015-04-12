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

#include "facebookusersummaryrequest.h"
#include "socialrequest_p.h"
#include <QtCore/QJsonObject>

class FacebookUserSummaryRequestPrivate: public SocialRequestPrivate
{
private:
    explicit FacebookUserSummaryRequestPrivate(FacebookUserSummaryRequest *q);
    QString userId;
private:
    Q_DECLARE_PUBLIC(FacebookUserSummaryRequest)
};

FacebookUserSummaryRequestPrivate::FacebookUserSummaryRequestPrivate(FacebookUserSummaryRequest *q)
    : SocialRequestPrivate(q)
{
}

FacebookUserSummaryRequest::FacebookUserSummaryRequest(QObject *parent)
    : AbstractFacebookRequest(*(new FacebookUserSummaryRequestPrivate(this)), parent)
{
}

QString FacebookUserSummaryRequest::userId() const
{
    Q_D(const FacebookUserSummaryRequest);
    return d->userId;
}

void FacebookUserSummaryRequest::setUserId(const QString &userId)
{
    Q_D(FacebookUserSummaryRequest);
    if (d->userId != userId) {
        d->userId = userId;
        emit userIdChanged();
    }
}

QVariantMap FacebookUserSummaryRequest::createMetadata(const SocialNetwork &socialNetwork, Mode mode,
                                                      const QVariantMap &metadata) const
{
    Q_D(const FacebookUserSummaryRequest);
    Q_UNUSED(socialNetwork);
    Q_UNUSED(mode);
    QVariantMap newMetadata = metadata;
    newMetadata.insert("userId", d->userId);
    newMetadata.insert("type", "User");
    return newMetadata;
}

QString FacebookUserSummaryRequest::queryId() const
{
    return "10153450790586729";
}

QJsonObject FacebookUserSummaryRequest::queryParameters(Mode mode, const QVariantMap &metadata) const
{
    Q_D(const FacebookUserSummaryRequest);
    Q_UNUSED(mode);
    QJsonObject returned;
    returned.insert("0", d->userId);
    returned.insert("1", "1080"); // Some image size
    returned.insert("2", "image/jpeg");
    returned.insert("3", "640"); // Some image size
    returned.insert("4", "2"); // ??? Some type ?
    returned.insert("5", "80"); // ???
    returned.insert("6", "image/x-auto");
    returned.insert("9", "false"); // ???
    returned.insert("10", "180"); // ???
    returned.insert("11", "240");
    returned.insert("12", "180");
    returned.insert("14", "ANDROID_TIMELINE");
    return returned;
}

QString FacebookUserSummaryRequest::requestName() const
{
    return "UserTimelineQueryWithContext";
}

QString FacebookUserSummaryRequest::apiCallerClass() const
{
    return "com.facebook.timeline.TimelineFragment";
}



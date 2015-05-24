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

#include "facebookfriendlistrequest.h"
#include "socialrequest_p.h"
#include <QtCore/QJsonObject>

class FacebookFriendListRequestPrivate: public SocialRequestPrivate
{
private:
    explicit FacebookFriendListRequestPrivate(FacebookFriendListRequest *q);
    QString userId;
    int count;
    int profilePictureSize;
private:
    Q_DECLARE_PUBLIC(FacebookFriendListRequest)
};

FacebookFriendListRequestPrivate::FacebookFriendListRequestPrivate(FacebookFriendListRequest *q)
    : SocialRequestPrivate(q), count(20), profilePictureSize(196)
{
}

FacebookFriendListRequest::FacebookFriendListRequest(QObject *parent)
    : AbstractFacebookRequest(*(new FacebookFriendListRequestPrivate(this)), parent)
{
}

QString FacebookFriendListRequest::userId() const
{
    Q_D(const FacebookFriendListRequest);
    return d->userId;
}

void FacebookFriendListRequest::setUserId(const QString &userId)
{
    Q_D(FacebookFriendListRequest);
    if (d->userId != userId) {
        d->userId = userId;
        emit userIdChanged();
    }
}

int FacebookFriendListRequest::count() const
{
    Q_D(const FacebookFriendListRequest);
    return d->count;
}

void FacebookFriendListRequest::setCount(int count)
{
    Q_D(FacebookFriendListRequest);
    if (d->count != count) {
        d->count = count;
        emit countChanged();
    }
}

int FacebookFriendListRequest::profilePictureSize() const
{
    Q_D(const FacebookFriendListRequest);
    return d->profilePictureSize;
}

void FacebookFriendListRequest::setProfilePictureSize(int profilePictureSize)
{
    Q_D(FacebookFriendListRequest);
    if (d->profilePictureSize != profilePictureSize) {
        d->profilePictureSize = profilePictureSize;
        emit profilePictureSizeChanged();
    }
}

QVariantMap FacebookFriendListRequest::createMetadata(const SocialNetwork &socialNetwork, Mode mode,
                                                      const QVariantMap &metadata) const
{
    Q_D(const FacebookFriendListRequest);
    Q_UNUSED(socialNetwork);
    Q_UNUSED(mode);
    QVariantMap newMetadata = metadata;
    newMetadata.insert("userId", d->userId);
    newMetadata.insert("type", "User");
    newMetadata.insert("dataRoot", "friends");
    return newMetadata;
}

QString FacebookFriendListRequest::queryId() const
{
    return "10153218947031729";
}

QJsonObject FacebookFriendListRequest::queryParameters(const Facebook &facebook, Mode mode,
                                                       const QVariantMap &metadata) const
{
    Q_D(const FacebookFriendListRequest);
    Q_UNUSED(facebook);
    QJsonObject returned;
    returned.insert("0", d->userId);
    returned.insert("1", ""); // ??? Maybe for startCursor ?
    if (mode == LoadNext) {
        returned.insert("2", metadata.value("endCursor").toString());
    }
    returned.insert("3", QString::number(d->count));
    returned.insert("4", QString::number(d->profilePictureSize));
    return returned;
}

QString FacebookFriendListRequest::requestName() const
{
    return "FetchFriendListQuery";
}

QString FacebookFriendListRequest::apiCallerClass() const
{
    return "com.facebook.friendlist.protocol.FetchFriendListGraphQLModels$FetchFriendListQueryModel";
}

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

#ifndef FACEBOOKFRIENDLISTREQUEST_H
#define FACEBOOKFRIENDLISTREQUEST_H

#include "abstractfacebookrequest.h"

class FacebookFriendListRequestPrivate;
class FacebookFriendListRequest : public AbstractFacebookRequest
{
    Q_OBJECT
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(int profilePictureSize READ profilePictureSize WRITE setProfilePictureSize
               NOTIFY profilePictureSizeChanged)
public:
    explicit FacebookFriendListRequest(QObject *parent = 0);
    QString userId() const;
    void setUserId(const QString &userId);
    int count() const;
    void setCount(int count);
    int profilePictureSize() const;
    void setProfilePictureSize(int profilePictureSize);
Q_SIGNALS:
    void userIdChanged();
    void countChanged();
    void profilePictureSizeChanged();
protected:
    QVariantMap createMetadata(const SocialNetwork &socialNetwork, Mode mode,
                               const QVariantMap &metadata) const Q_DECL_OVERRIDE;
    QString queryId() const Q_DECL_OVERRIDE;
    QJsonObject queryParameters(const Facebook &facebook, Mode mode,
                                const QVariantMap &metadata) const Q_DECL_OVERRIDE;
    QString requestName() const Q_DECL_OVERRIDE;
    QString apiCallerClass() const Q_DECL_OVERRIDE;
private:
    Q_DECLARE_PRIVATE(FacebookFriendListRequest)
};

#endif // FACEBOOKFRIENDLISTREQUEST_H

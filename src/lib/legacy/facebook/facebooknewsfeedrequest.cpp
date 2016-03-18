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

#include "facebooknewsfeedrequest.h"
#include "socialrequest_p.h"
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include <QtCore/QUuid>
#include "facebook.h"

class FacebookNewsFeedRequestPrivate: public SocialRequestPrivate
{
public:
    explicit FacebookNewsFeedRequestPrivate(FacebookNewsFeedRequest *q);
};

FacebookNewsFeedRequestPrivate::FacebookNewsFeedRequestPrivate(FacebookNewsFeedRequest *q)
    : SocialRequestPrivate(q)
{
}

FacebookNewsFeedRequest::FacebookNewsFeedRequest(QObject *parent)
    : AbstractFacebookRequest(*(new FacebookNewsFeedRequestPrivate(this)), parent)
{
}

QString FacebookNewsFeedRequest::queryId() const
{
    return "10153472222246729";
}

QJsonObject FacebookNewsFeedRequest::queryParameters(const Facebook &facebook, SocialRequest::Mode mode,
                                                     const QVariantMap &metadata) const
{
    Q_UNUSED(mode);
    QJsonObject returned;
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);
    QString uuid = QUuid::createUuid().toString().remove("{").remove("}");
    returned.insert("0", QString("%1_%2_%3").arg(facebook.userId(), timestamp, uuid));
    returned.insert("1", QString("{is_charging:\"false\",battery_level:100}")); // Battery level
    returned.insert("2", QString("GOOD")); // Signal strength
    returned.insert("3", QString("")); // Device id ???
    returned.insert("5", QJsonArray()); // ???
    returned.insert("6", QString("auto")); // ???
    returned.insert("7", QString("top_stories"));
    if (mode == LoadNext) {
        returned.insert("9", metadata.value("endCursor").toString());
    }

    returned.insert("10", 10); // ???
    returned.insert("11", 160);
    returned.insert("12", QString("image/jpeg"));
    returned.insert("13", 540);
    returned.insert("14", 1080);
    returned.insert("15", 1080);
    returned.insert("16", 559); // ???
    returned.insert("17", 2048);
    returned.insert("18", QString("contain-fit"));
    returned.insert("19", QString("false")); // ???

    returned.insert("20", QString("true")); // ???



    returned.insert("24", 960);
    returned.insert("25", 50); // Small number: ???
    returned.insert("26", 120);
    returned.insert("27", 120);
    returned.insert("28", 160);
    returned.insert("29", 160);

    returned.insert("30", 120);

    returned.insert("32", QString("image/x-auto"));
    returned.insert("33", 480);
    returned.insert("34", QString("image/x-auto"));
    returned.insert("35", QString("true")); // ???
    returned.insert("36", QString("2")); // ???
    returned.insert("37", 480);
    returned.insert("38", 1080);
    returned.insert("39", 540);

    returned.insert("40", 120);
    returned.insert("41", 4); // ???
    returned.insert("42", 60); // Small number: ???

    returned.insert("44", 160);
    returned.insert("45", QString("feed"));
    returned.insert("46", QString("ANDROID_STORY"));
    returned.insert("47", QString("PRODUCTION"));
    returned.insert("48", QString("false")); // ???
    returned.insert("49", QString("true")); // ???


    returned.insert("51", 320);
    returned.insert("52", 960);
    returned.insert("53", 480);
    returned.insert("54", 480);
    returned.insert("55", 80); // Small number: ???
    returned.insert("56", 160);
    returned.insert("57", 540);
    returned.insert("58", 560);
    returned.insert("59", 30); // Small number: ???

    returned.insert("60", 2048);
    returned.insert("61", 2048);
    returned.insert("62", 2048);
    returned.insert("63", 120);
    returned.insert("likers_profile_image_size", 12);
    return returned;

}

QString FacebookNewsFeedRequest::requestName() const
{
    return "NewsFeedQueryDepth3";
}

QString FacebookNewsFeedRequest::apiCallerClass() const
{
    return "com.facebook.feed.fragment.NewsFeedFragment";
}


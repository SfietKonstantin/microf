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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtQml/qqml.h>
#include "socialcontentitem.h"
#include "socialcontentmodel.h"
#include "facebook/facebook.h"
#include "facebook/facebookloginrequest.h"
#include "facebook/facebooklogincontentbuilder.h"
#include "facebook/facebooklogoutrequest.h"
#include "facebook/facebooklistproperty.h"
#include "facebook/facebookitembuilder.h"
#include "facebook/facebookmodelbuilder.h"
#include "facebook/facebooknewsfeedmodelbuilder.h"
#include "facebook/facebookconfirmationcontentbuilder.h"
#include "facebook/facebookfriendlistrequest.h"
#include "facebook/facebookusersummaryrequest.h"
#include "facebook/facebooknewsfeedrequest.h"

static const char *ABSTRACT_TYPE = "Abstract type cannot be used";
static const char *GLOBAL_TYPE = "Global type cannot be used";

void registerMicroFTypes(const char *uri)
{
    qmlRegisterUncreatableType<SocialNetwork>(uri, 1, 0, "SocialNetwork", ABSTRACT_TYPE);
    qmlRegisterUncreatableType<SocialNetworkStatus>(uri, 1, 0, "SocialNetworkStatus", GLOBAL_TYPE);
    qmlRegisterUncreatableType<SocialNetworkError>(uri, 1, 0, "SocialNetworkError", GLOBAL_TYPE);
    qmlRegisterUncreatableType<SocialRequest>(uri, 1, 0, "SocialRequest", ABSTRACT_TYPE);
    qmlRegisterUncreatableType<SocialContentItemBuilder>(uri, 1, 0, "SocialContentItemBuilder", ABSTRACT_TYPE);
    qmlRegisterUncreatableType<SocialContentModelBuilder>(uri, 1, 0, "SocialContentModelBuilder", ABSTRACT_TYPE);
    qmlRegisterType<SocialContentItem>(uri, 1, 0, "SocialContentItem");
    qmlRegisterType<SocialContentModel>(uri, 1, 0, "SocialContentModel");
    qmlRegisterType<Facebook>(uri, 1, 0, "Facebook");
    qmlRegisterType<FacebookLoginRequest>(uri, 1, 0, "FacebookLoginRequest");
    qmlRegisterType<FacebookLoginContentBuilder>(uri, 1, 0, "FacebookLoginContentBuilder");
    qmlRegisterType<FacebookLogoutRequest>(uri, 1, 0, "FacebookLogoutRequest");
    qmlRegisterType<FacebookConfirmationContentBuilder>(uri, 1, 0, "FacebookConfirmationContentBuilder");
    qmlRegisterType<FacebookProperty>(uri, 1, 0, "FacebookProperty");
    qmlRegisterType<FacebookListProperty>(uri, 1, 0, "FacebookListProperty");
    qmlRegisterType<FacebookItemBuilder>(uri, 1, 0, "FacebookItemBuilder");
    qmlRegisterType<FacebookModelBuilder>(uri, 1, 0, "FacebookModelBuilder");
    qmlRegisterType<FacebookNewsFeedModelBuilder>(uri, 1, 0, "FacebookNewsFeedModelBuilder");
    qmlRegisterType<FacebookFriendListRequest>(uri, 1, 0, "FacebookFriendListRequest");
    qmlRegisterType<FacebookUserSummaryRequest>(uri, 1, 0, "FacebookUserSummaryRequest");
    qmlRegisterType<FacebookNewsFeedRequest>(uri, 1, 0, "FacebookNewsFeedRequest");
}

#endif // PLUGIN_H


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

#include "threadsrequestfactory.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include "icontroller.h"
#include "facebook.h"
#include "facebookutils.h"

namespace microf { namespace internal {

ThreadsRequestFactory::ThreadsRequestFactory(IController &parent)
    : m_parent {parent}
{
}

std::unique_ptr<ThreadsRequestFactory::Job_t> ThreadsRequestFactory::create(EmptyRequest &&request) const
{
    Q_UNUSED(request);
    return std::unique_ptr<Job_t>(new Job(m_parent));
}

ThreadsRequestFactory::Job::Job(IController &parent)
    : m_parent {parent}
{
}

void ThreadsRequestFactory::Job::execute(Job::OnResult_t onResult, Job::OnError_t onError)
{
    Q_UNUSED(onError)
    Q_ASSERT(m_parent.facebook());
    FacebookUtils utils {*(m_parent.facebook())};
    QJsonObject requestJson {
        {"2", "true"},
        {"12", "false"},
        {"13", "false"}
    };
    QByteArray postData {utils.createPostData("10153919752026729", "ThreadQuery", requestJson)};
    onResult(HttpRequest(HttpRequest::Type::Post, utils.createHttpRequest(postData), postData));
}

}}

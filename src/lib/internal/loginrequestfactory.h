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

#ifndef MICROF_INTERNAL_LOGINREQUESTFACTORY_H
#define MICROF_INTERNAL_LOGINREQUESTFACTORY_H

#include <core/ijobfactory.h>
#include <http/httprequest.h>
#include <error/error.h>
#include "core/loginrequest.h"

namespace microf {

class SessionController;
namespace internal {

using ::microcore::core::IJobFactory;
using ::microcore::core::IJob;
using ::microcore::http::HttpRequest;
using ::microcore::error::Error;
using ::microcore::fb::LoginRequest;

class LoginRequestFactory: public IJobFactory<LoginRequest, HttpRequest, Error>
{
public:
    using Job_t = IJob<HttpRequest, Error>;
    explicit LoginRequestFactory(SessionController &parent);
    std::unique_ptr<Job_t> create(LoginRequest &&request) const override;
private:
    class Job : public Job_t
    {
    public:
        explicit Job(LoginRequest &&request, SessionController &parent);
        void execute(OnResult_t onResult, OnError_t onError) override;
    private:
        QByteArray createPostData() const;
        LoginRequest m_request {};
        SessionController &m_parent;
    };
    SessionController &m_parent;
};

}}

#endif // MICROF_INTERNAL_LOGINREQUESTFACTORY_H

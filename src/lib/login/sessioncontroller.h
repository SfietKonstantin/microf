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

#ifndef MICROF_LOGIN_LOGINCONTROLLER_H
#define MICROF_LOGIN_LOGINCONTROLLER_H

#include <qt/viewitemcontroller.h>
#include "core/session.h"
#include "data/item.h"

namespace microf {

class Facebook;

namespace login {

class SessionController : public ::microcore::qt::ViewItemController< ::microcore::data::Item< ::microcore::fb::Session>>
{
    Q_OBJECT
    Q_PROPERTY(microf::Facebook * facebook READ facebook WRITE setFacebook NOTIFY facebookChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(QString machineId READ machineId WRITE setMachineId NOTIFY machineIdChanged)
public:
    explicit SessionController(QObject *parent = nullptr);
    ::microcore::data::Item< ::microcore::fb::Session> & item() override;
    ::microf::Facebook * facebook() const;
    void setFacebook(::microf::Facebook *facebook);

    QString email() const;
    void setEmail(const QString &email);
    QString password() const;
    void setPassword(const QString &password);
    QString deviceId() const;
    void setDeviceId(const QString &deviceId);
    QString machineId() const;
    void setMachineId(const QString &machineId);
public Q_SLOTS:
    bool login();
Q_SIGNALS:
    void facebookChanged();
    void emailChanged();
    void passwordChanged();
    void deviceIdChanged();
    void machineIdChanged();
private:
    using Executor_t = ::microcore::core::Executor<Error_t>;
    ::microcore::data::Item< ::microcore::fb::Session> m_data {};
    ::microf::Facebook * m_facebook {nullptr};
    QString m_email {};
    QString m_password {};
    QString m_deviceId {};
    QString m_machineId {};
    Executor_t *m_loginExecutor {nullptr};
};

}}

#endif // MICROF_LOGIN_LOGINCONTROLLER_H

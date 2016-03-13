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

#include <QtCore/QRegularExpression>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include "facebook.h"
#include "core/sessionobject.h"
#include "login/sessioncontroller.h"
#include "qt/viewitem.h"
#include "authhelper.h"

using SessionViewItem = ::microcore::qt::ViewItem<::microcore::data::Item< ::microcore::fb::Session>, ::microcore::fb::qt::SessionObject>;

static void registerTypes()
{
    qmlRegisterUncreatableType< ::microcore::qt::ViewController>("org.sfietkonstantin.microf", 1, 0, "ViewController", "Uncreatable");
    qmlRegisterType< ::microf::Facebook>("org.sfietkonstantin.microf", 1, 0, "Facebook");
    qmlRegisterType< ::microcore::fb::qt::SessionObject>("org.sfietkonstantin.microf", 1, 0, "Session");
    qmlRegisterType< ::microf::login::SessionController>("org.sfietkonstantin.microf", 1, 0, "SessionController");
    qmlRegisterType<SessionViewItem>("org.sfietkonstantin.microf", 1, 0, "SessionViewItem");


    qmlRegisterType<AuthHelper>("org.sfietkonstantin.microf", 1, 0, "AuthHelper");
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    registerTypes();
    app.setOrganizationName("microf");
    app.setApplicationName("fidller");
    QQmlApplicationEngine engine (QUrl("qrc:/main.qml"));
    Q_UNUSED(engine);
    return app.exec();
}

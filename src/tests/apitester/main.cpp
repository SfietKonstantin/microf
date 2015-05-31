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

#include <QtCore/QRegularExpression>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <plugin.h>
#include "authhelper.h"
#include "infohelper.h"
#include "requesthelpermodel.h"
#include "metapropertyhelpermodel.h"
#include "customfacebookrequest.h"
#include "customfacebookrequest.h"
#include "buildershelpermodel.h"

template<class T> void appendPropertyToBuilder(T *builder, const QString &path, const QString &name)
{
    QQmlListProperty<FacebookProperty> properties = builder->properties();
    FacebookProperty *property = new FacebookProperty(builder);
    property->setPath(path);
    property->setName(name);
    properties.append(&properties, property);
}

static void appendProperty(FacebookItemBuilder *itemBuilder,
                           AbstractFacebookModelBuilder *modelBuilder,
                           const QString &path, const QString &name)
{
    if (itemBuilder) {
        appendPropertyToBuilder(itemBuilder, path, name);
    }
    if (modelBuilder) {
        appendPropertyToBuilder(modelBuilder, path, name);
    }
}

template<class T> void appendListPropertyToBuilder(T *builder, const QString &path,
                                                   const QString &name,
                                                   const QMap<QString, QString> &mapping)
{
    QQmlListProperty<FacebookProperty> properties = builder->properties();
    FacebookListProperty *property = new FacebookListProperty(builder);
    property->setPath(path);
    property->setName(name);

    QQmlListProperty<FacebookProperty> listProperties = property->properties();
    for (const QString &path : mapping.keys()) {
        FacebookProperty *listProperty = new FacebookProperty();
        listProperty->setName(mapping.value(path));
        listProperty->setPath(path);
        listProperties.append(&listProperties, listProperty);
    }
    properties.append(&properties, property);
}

static void appendListProperty(FacebookItemBuilder *itemBuilder,
                               AbstractFacebookModelBuilder *modelBuilder,
                               const QString &path, const QString &name,
                               const QMap<QString, QString> &mapping)
{
    if (itemBuilder) {
        appendListPropertyToBuilder(itemBuilder, path, name, mapping);
    }
    if (modelBuilder) {
        appendListPropertyToBuilder(modelBuilder, path, name, mapping);
    }
}

class Helper: public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = 0): QObject(parent) {}
public slots:
    void setBuilderProperties(const QString &properties, FacebookItemBuilder *itemBuilder,
                              AbstractFacebookModelBuilder *modelBuilder)
    {
        if (itemBuilder) {
            QQmlListProperty<FacebookProperty> properties = itemBuilder->properties();
            properties.clear(&properties);
        }
        if (modelBuilder) {
            QQmlListProperty<FacebookProperty> properties = modelBuilder->properties();
            properties.clear(&properties);
        }

        QStringList splitted = properties.trimmed().split("\n");

        QRegularExpression mainRegExp ("^([a-zA-Z_/]+)(\\[[a-zA-Z_/:,]+\\])?:([a-zA-Z_]+)$");
        QRegularExpression listRegExp ("^([a-zA-Z_/]+):([a-zA-Z_]+)$");

        for (const QString &line : splitted) {
            QRegularExpressionMatch match = mainRegExp.match(line.trimmed());
            if (!match.isValid()) {
                continue;
            }

            QStringList captured = match.capturedTexts();
            QString path;
            QString listMapping;
            QString name;
            if (captured.count() == 3) {
                path = captured.at(1);
                name = captured.at(2);
            } else if (captured.count() == 4) {
                path = captured.at(1);
                listMapping = captured.at(2);
                name = captured.at(3);
            } else {
                continue;
            }

            if (path.isEmpty() || name.isEmpty()) {
                continue;
            }

            if (listMapping.isEmpty()) {
                appendProperty(itemBuilder, modelBuilder, path, name);
            } else {
                QMap<QString, QString> mapping;
                QStringList listSplitted = listMapping.remove('[').remove(']').split(",");
                for (const QString &entry : listSplitted) {
                    QRegularExpressionMatch listMatch = listRegExp.match(entry.trimmed());
                    QString listPath = listMatch.captured(1);
                    QString listName = listMatch.captured(2);

                    if (!listPath.isEmpty() && !listName.isEmpty()) {
                        mapping.insert(listPath, listName);
                    }
                }
                if (mapping.isEmpty()) {
                    continue;
                }

                appendListProperty(itemBuilder, modelBuilder, path, name, mapping);
            }
        }
    }
};

static QObject * helper_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new Helper();
}

static void registerTypes()
{
    registerMicroFTypes("org.sfietkonstantin.microf");
    qmlRegisterType<AuthHelper>("org.sfietkonstantin.microf", 1, 0, "AuthHelper");
    qmlRegisterType<InfoHelper>("org.sfietkonstantin.microf", 1, 0, "InfoHelper");
    qmlRegisterType<RequestHelperModel>("org.sfietkonstantin.microf", 1, 0, "RequestHelperModel");
    qmlRegisterType<MetaPropertyHelperModel>("org.sfietkonstantin.microf", 1, 0, "MetaPropertyHelperModel");
    qmlRegisterType<CustomFacebookRequest>("org.sfietkonstantin.microf", 1, 0, "CustomFacebookRequest");
    qmlRegisterType<BuildersHelperModel>("org.sfietkonstantin.microf", 1, 0, "BuildersHelperModel");
    qmlRegisterSingletonType<Helper>("org.sfietkonstantin.microf", 1, 0, "Helper", helper_singleton);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    registerTypes();
    app.setOrganizationName("microf");
    app.setApplicationName("apitester");
    QQmlApplicationEngine engine (QUrl("qrc:/main.qml"));
    Q_UNUSED(engine);
    return app.exec();
}

#include "main.moc"

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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QSharedPointer>
#include <QtCore/QTemporaryDir>
#include "socialcontentitembuilder.h"
#include "socialcontentitem.h"
#include "socialcontentmodel.h"
#include "socialcontentmodelbuilder.h"
#include "socialnetwork.h"
#include "socialrequest.h"

static const char *NPM_EXEC = "/usr/bin/npm";
static const char *NPM_INSTALL_ARGS = "install";
static const char *NODE_EXEC = "/usr/bin/node";
static const char *NODE_APP_ARGS = "app.js";

class TstSocialNetwork: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void tstSimple();
    void tstError();
    void tstSimpleList();
private:
    QSharedPointer<QTemporaryDir> m_tempDir;
    QSharedPointer<QProcess> m_process;
};

class SimpleRequest: public SocialRequest
{
public:
    explicit SimpleRequest(const QUrl &url, QObject *parent = 0)
        : SocialRequest(parent), m_url(url)
    {
    }
    Type type() const
    {
        return SocialRequest::Post;
    }
protected:
    QNetworkRequest createRequest(const SocialNetwork &socialNetwork,
                                  const QByteArray &postData, Mode mode,
                                  const QVariantMap &metadata) const override
    {
        Q_UNUSED(socialNetwork);
        Q_UNUSED(postData);
        Q_UNUSED(mode);
        Q_UNUSED(metadata);
        QNetworkRequest request;
        request.setRawHeader("Content-Type", "application/json;charset=UTF-8");
        request.setUrl(m_url);
        return request;
    }
    QByteArray createPostData(const SocialNetwork &socialNetwork, Mode mode,
                              const QVariantMap &metadata) const override
    {
        Q_UNUSED(socialNetwork);
        Q_UNUSED(mode);

        if (!metadata.isEmpty()) {
            QJsonObject object = QJsonObject::fromVariantMap(metadata);
            QJsonDocument document;
            document.setObject(object);
            return document.toJson(QJsonDocument::Compact);
        }
        return QByteArray();
    }
private:
    QUrl m_url;
};

class SimpleBuilder: public SocialContentItemBuilder
{
public:
    explicit SimpleBuilder(QObject *parent = 0)
        : SocialContentItemBuilder(parent)
    {
    }
protected:
    void build(SocialContentItem &contentItem, QNetworkReply::NetworkError error,
               const QString &errorMessage, const QByteArray &data,
               const QVariantMap &metadata) override
    {
        Q_UNUSED(metadata);
        if (error != QNetworkReply::NoError) {
            setError(contentItem, SocialNetworkError::Network, errorMessage);
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(data);
        if (!document.isObject()) {
            setError(contentItem, SocialNetworkError::Data, "Cannot convert to JSON");
            return;
        }

        setObject(contentItem, document.object().toVariantMap());
    }
};

class SimpleModelBuilder: public SocialContentModelBuilder
{
public:
    explicit SimpleModelBuilder(QObject *parent = 0)
        : SocialContentModelBuilder(parent)
    {
    }
protected:
    void build(SocialContentModel &contentModel, QNetworkReply::NetworkError error,
               const QString &errorMessage, const QByteArray &data,
               const QVariantMap &metadata) override
    {
        Q_UNUSED(metadata);
        if (error != QNetworkReply::NoError) {
            setError(contentModel, SocialNetworkError::Network, errorMessage);
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(data);
        if (!document.isObject()) {
            setError(contentModel, SocialNetworkError::Data, "Cannot convert to JSON");
            return;
        }
        const QJsonObject object = document.object();

        bool hasNext = object.value("has_next").toBool(false);
        QJsonArray array = object.value("values").toArray();

        QList<QVariantMap> modelData;
        for (const QJsonValue &value : array) {
            modelData.append(value.toObject().toVariantMap());
        }

        QVariantMap newMetadata;
        newMetadata.insert("next", hasNext);
        setData(contentModel, modelData, hasNext, false, newMetadata);
    }
};

void TstSocialNetwork::initTestCase()
{
    // Install the server
    m_tempDir = QSharedPointer<QTemporaryDir>(new QTemporaryDir());
    QVERIFY(m_tempDir->isValid());

    QDir res (":/");
    QDir tmp (m_tempDir->path());
    for (const QString &fileName : res.entryList(QDir::Files)) {
        QFile file (res.absoluteFilePath(fileName));
        QVERIFY(file.copy(tmp.absoluteFilePath(fileName)));
    }
    QProcess process;
    process.setWorkingDirectory(m_tempDir->path());
    QStringList npmArgs;
    npmArgs.append(NPM_INSTALL_ARGS);
    process.start(NPM_EXEC, npmArgs);
    QVERIFY(process.waitForFinished(-1));
    QCOMPARE(process.exitCode(), 0);

    m_process = QSharedPointer<QProcess>(new QProcess());
    m_process->setWorkingDirectory(m_tempDir->path());
    QStringList nodeArgs;
    nodeArgs.append(NODE_APP_ARGS);
    m_process->start(NODE_EXEC, nodeArgs);
    QVERIFY(m_process->waitForStarted(-1));

    QTest::qWait(2000); // Wait for Node to start
}

void TstSocialNetwork::tstSimple()
{
    SocialContentItem socialContentItem;
    socialContentItem.classBegin();
    socialContentItem.componentComplete();

    QVERIFY(!socialContentItem.load()); // No social network

    QSignalSpy socialNetworkSpy (&socialContentItem, SIGNAL(socialNetworkChanged()));
    SocialNetwork socialNetwork;
    socialNetwork.classBegin();
    socialNetwork.componentComplete();
    socialContentItem.setSocialNetwork(&socialNetwork);
    QCOMPARE(socialContentItem.socialNetwork(), &socialNetwork);
    QCOMPARE(socialNetworkSpy.count(), 1);

    QVERIFY(!socialContentItem.load()); // No request

    QSignalSpy requestSpy (&socialContentItem, SIGNAL(requestChanged()));
    SimpleRequest request (QUrl("http://localhost:8080/api/postsimple"));
    request.classBegin();
    request.componentComplete();
    socialContentItem.setRequest(&request);
    QCOMPARE(socialContentItem.request(), &request);
    QCOMPARE(requestSpy.count(), 1);

    QVERIFY(!socialContentItem.load()); // No builder

    QSignalSpy builderSpy (&socialContentItem, SIGNAL(builderChanged()));
    SimpleBuilder builder;
    builder.classBegin();
    builder.componentComplete();
    socialContentItem.setBuilder(&builder);
    QCOMPARE(socialContentItem.builder(), &builder);
    QCOMPARE(builderSpy.count(), 1);

    QSignalSpy statusSpy (&socialContentItem, SIGNAL(statusChanged()));
    QVERIFY(socialContentItem.load());
    QCOMPARE(statusSpy.count(), 1);
    statusSpy.clear();

    QVERIFY(!socialContentItem.load()); // Busy

    while (statusSpy.count() < 1) {
        QTest::qWait(200);
    }

    QCOMPARE(socialContentItem.status(), SocialNetworkStatus::Ready);

    // Check the object
    const QVariantMap &object = socialContentItem.object();
    QCOMPARE(object.value("text"), QVariant("Hello world"));
}

void TstSocialNetwork::tstError()
{
    SocialContentItem socialContentItem;
    socialContentItem.classBegin();
    socialContentItem.componentComplete();

    SocialNetwork socialNetwork;
    socialNetwork.classBegin();
    socialNetwork.componentComplete();
    socialContentItem.setSocialNetwork(&socialNetwork);

    SimpleRequest request (QUrl("http://localhost:8080/api/postsimpleerror"));
    request.classBegin();
    request.componentComplete();
    socialContentItem.setRequest(&request);

    SimpleBuilder builder;
    builder.classBegin();
    builder.componentComplete();
    socialContentItem.setBuilder(&builder);

    QSignalSpy statusSpy (&socialContentItem, SIGNAL(statusChanged()));
    QVERIFY(socialContentItem.load());
    QCOMPARE(statusSpy.count(), 1);
    statusSpy.clear();

    while (statusSpy.count() < 1) {
        QTest::qWait(200);
    }

    QCOMPARE(socialContentItem.status(), SocialNetworkStatus::Error);
    QCOMPARE(socialContentItem.error(), SocialNetworkError::Network);
    QVERIFY(!socialContentItem.errorMessage().isEmpty());
}

void TstSocialNetwork::tstSimpleList()
{
    SocialContentModel socialContentModel;
    socialContentModel.classBegin();
    socialContentModel.componentComplete();

    QVERIFY(!socialContentModel.load()); // No social network

    QSignalSpy socialNetworkSpy (&socialContentModel, SIGNAL(socialNetworkChanged()));
    SocialNetwork socialNetwork;
    socialNetwork.classBegin();
    socialNetwork.componentComplete();
    socialContentModel.setSocialNetwork(&socialNetwork);
    QCOMPARE(socialContentModel.socialNetwork(), &socialNetwork);
    QCOMPARE(socialNetworkSpy.count(), 1);

    QVERIFY(!socialContentModel.load()); // No request

    QSignalSpy requestSpy (&socialContentModel, SIGNAL(requestChanged()));
    SimpleRequest request (QUrl("http://localhost:8080/api/postsimplelist"));
    request.classBegin();
    request.componentComplete();
    socialContentModel.setRequest(&request);
    QCOMPARE(socialContentModel.request(), &request);
    QCOMPARE(requestSpy.count(), 1);

    QVERIFY(!socialContentModel.load()); // No builder

    QSignalSpy builderSpy (&socialContentModel, SIGNAL(builderChanged()));
    SimpleModelBuilder builder;
    builder.classBegin();
    builder.componentComplete();
    socialContentModel.setBuilder(&builder);
    QCOMPARE(socialContentModel.builder(), &builder);
    QCOMPARE(builderSpy.count(), 1);

    QSignalSpy statusSpy (&socialContentModel, SIGNAL(statusChanged()));
    QSignalSpy modelSpy (&socialContentModel, SIGNAL(countChanged()));
    QVERIFY(socialContentModel.load());
    QCOMPARE(statusSpy.count(), 1);
    statusSpy.clear();

    QVERIFY(!socialContentModel.load()); // Busy

    while (statusSpy.count() < 1) {
        QTest::qWait(200);
    }

    QCOMPARE(modelSpy.count(), 1);
    QCOMPARE(socialContentModel.status(), SocialNetworkStatus::Ready);

    // Check the model
    QCOMPARE(socialContentModel.count(), 2);
    QVariant value0 = socialContentModel.data(socialContentModel.index(0),
                                              SocialContentModel::ObjectRole);
    QVariantMap object0 = value0.toMap();
    QCOMPARE(object0.value("id"), QVariant(0));
    QCOMPARE(object0.value("text"), QVariant("Entry 0"));

    QVariant value1 = socialContentModel.data(socialContentModel.index(1),
                                              SocialContentModel::ObjectRole);
    QVariantMap object1 = value1.toMap();
    QCOMPARE(object1.value("id"), QVariant(1));
    QCOMPARE(object1.value("text"), QVariant("Entry 1"));

    QVERIFY(socialContentModel.hasNext());
    QVERIFY(!socialContentModel.hasPrevious());

    // Load next
    QVERIFY(socialContentModel.loadNext());
    QCOMPARE(statusSpy.count(), 2);
    statusSpy.clear();
    modelSpy.clear();

    while (statusSpy.count() < 1) {
        QTest::qWait(200);
    }

    QCOMPARE(modelSpy.count(), 1);
    QCOMPARE(socialContentModel.status(), SocialNetworkStatus::Ready);

    // Check the model
    QCOMPARE(socialContentModel.count(), 4);
    QVariant newValue0 = socialContentModel.data(socialContentModel.index(0),
                                              SocialContentModel::ObjectRole);
    QVariantMap newObject0 = newValue0.toMap();
    QCOMPARE(newObject0.value("id"), QVariant(0));
    QCOMPARE(newObject0.value("text"), QVariant("Entry 0"));

    QVariant newValue1 = socialContentModel.data(socialContentModel.index(1),
                                              SocialContentModel::ObjectRole);
    QVariantMap newObject1 = newValue1.toMap();
    QCOMPARE(newObject1.value("id"), QVariant(1));
    QCOMPARE(newObject1.value("text"), QVariant("Entry 1"));

    QVariant newValue2 = socialContentModel.data(socialContentModel.index(2),
                                              SocialContentModel::ObjectRole);
    QVariantMap newObject2 = newValue2.toMap();
    QCOMPARE(newObject2.value("id"), QVariant(2));
    QCOMPARE(newObject2.value("text"), QVariant("Entry 2"));

    QVariant newValue3 = socialContentModel.data(socialContentModel.index(3),
                                              SocialContentModel::ObjectRole);
    QVariantMap newObject3 = newValue3.toMap();
    QCOMPARE(newObject3.value("id"), QVariant(3));
    QCOMPARE(newObject3.value("text"), QVariant("Entry 3"));

    QVERIFY(!socialContentModel.hasNext());
    QVERIFY(!socialContentModel.hasPrevious());
}

QTEST_MAIN(TstSocialNetwork)

#include "tst_socialnetwork.moc"


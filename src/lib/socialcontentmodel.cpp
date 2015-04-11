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

#include "socialcontentmodel.h"
#include "socialcontentmodel_p.h"
#include <QtCore/QDebug>
#include "socialnetwork_p.h"
#include "socialobject_p.h"
#include "socialcontentmodelbuilder_p.h"

SocialContentModelPrivate::SocialContentModelPrivate(SocialContentModel *q)
    : AbstractSocialContentPrivate(q), m_haveNext(false), m_havePrevious(false)
    , m_updateMode(Replace), m_socialNetwork(0), m_request(0), m_builder(0)
{
}

void SocialContentModelPrivate::setContentModelData(SocialContentModel &contentModel,
                                                    const QList<QVariantMap> &data,
                                                    bool haveNext, bool havePrevious,
                                                    const QVariantMap &metadata)
{
    contentModel.d_func()->setContentModelData(data, haveNext, havePrevious, metadata);
}

void SocialContentModelPrivate::setContentModelError(SocialContentModel &contentModel,
                                                     SocialNetworkError::type error,
                                                     const QString &errorString)
{
    contentModel.d_func()->setError(error, errorString);
}

bool SocialContentModelPrivate::build(QNetworkReply::NetworkError error, const QString &errorString,
                                      const QByteArray &data)
{
    Q_Q(SocialContentModel);
    if (!m_builder) {
        return false;
    }
    SocialContentModelBuilderPrivate::build(*m_builder, *q, error, errorString, data);
    return true;
}

bool SocialContentModelPrivate::load(SocialContentModelPrivate::UpdateMode updateMode)
{
    if (status == SocialNetworkStatus::Busy) {
        qWarning() << "SocialContentModelPrivate::load() called while status is Busy";
        return false;
    }

    if (!m_socialNetwork) {
        qWarning() << "SocialContentModelPrivate::load() called without socialNetwork";
        return false;
    }

    if (!m_request) {
        qWarning() << "SocialContentModelPrivate::load() called without request";
        return false;
    }

    if (!m_builder) {
        qWarning() << "SocialContentModelPrivate::load() called without builder";
        return false;
    }

    bool ok = SocialNetworkPrivate::socialContentLoad(*m_socialNetwork, *this, *m_request);
    if (ok) {
        m_updateMode = updateMode;
        setStatus(SocialNetworkStatus::Busy);
    }
    return ok;
}

void SocialContentModelPrivate::setContentModelData(const QList<QVariantMap> &data,
                                                    bool haveNext, bool havePrevious,
                                                    const QVariantMap &metadata)
{
    Q_Q(SocialContentModel);
    QList<SocialObject *> newData;
    for (const QVariantMap &properties : data) {
        SocialObject *object = new SocialObject(q);
        for (const QString &key : properties.keys()) {
            const QVariant &value = properties.value(key);
            SocialObjectPrivate::setProperty(object, key.toLocal8Bit(), value);
        }
        newData.append(object);
    }

    switch (m_updateMode) {
    case Replace:
        setNewData(newData);
        break;
    case Append:
        appendNewData(newData);
        break;
    case Prepend:
        prependNewData(newData);
        break;
    }

    if (m_haveNext != haveNext) {
        m_haveNext = haveNext;
        emit q->haveNextChanged();
    }

    if (m_havePrevious != havePrevious) {
        m_havePrevious = havePrevious;
        emit q->havePreviousChanged();
    }

    setMetadata(metadata);
    setStatus(SocialNetworkStatus::Ready);
    emit q->finished(true);
}

void SocialContentModelPrivate::setNewData(const QList<SocialObject *> &data)
{
    Q_Q(SocialContentModel);
    if (data.isEmpty()) {
        return;
    }

    if (q->rowCount() > 0) {
        q->beginRemoveRows(QModelIndex(), 0, q->rowCount() - 1);
        qDeleteAll(m_data);
        m_data.clear();
        q->endRemoveRows();
    }

    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), 0, data.count() - 1);
        m_data = data;
        emit q->countChanged();
        q->endInsertRows();
    }
}

void SocialContentModelPrivate::appendNewData(const QList<SocialObject *> &data)
{
    Q_Q(SocialContentModel);
    if (data.isEmpty()) {
        return;
    }

    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), q->rowCount(), q->rowCount() + data.count() - 1);
        m_data.append(data);
        emit q->countChanged();
        q->endInsertRows();
    }
}

void SocialContentModelPrivate::prependNewData(const QList<SocialObject *> &data)
{
    Q_Q(SocialContentModel);
    if (data.isEmpty()) {
        return;
    }

    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), 0, data.count() - 1);
        QList<SocialObject *> oldData = m_data;
        m_data = data;
        m_data.append(oldData);
        emit q->countChanged();
        q->endInsertRows();
    }
}

SocialContentModel::SocialContentModel(QObject *parent)
    : QAbstractListModel(parent), d_ptr(new SocialContentModelPrivate(this))
{
}

SocialContentModel::~SocialContentModel()
{
    Q_D(SocialContentModel);
    qDeleteAll(d->m_data);
}

QHash<int, QByteArray> SocialContentModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ObjectRole, "object");
    return roles;
}

int SocialContentModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const SocialContentModel);
    Q_UNUSED(parent);
    return d->m_data.count();
}

QVariant SocialContentModel::data(const QModelIndex &index, int role) const
{
    Q_D(const SocialContentModel);
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    switch (role) {
    case ObjectRole:
        return QVariant::fromValue(d->m_data.value(row));
        break;
    default:
        return QVariant();
        break;
    }
}

void SocialContentModel::classBegin()
{
}

void SocialContentModel::componentComplete()
{
}

int SocialContentModel::count() const
{
    Q_D(const SocialContentModel);
    return d->m_data.count();
}

bool SocialContentModel::haveNext() const
{
    Q_D(const SocialContentModel);
    return d->m_haveNext;
}

bool SocialContentModel::havePrevious() const
{
    Q_D(const SocialContentModel);
    return d->m_havePrevious;
}

SocialNetwork * SocialContentModel::socialNetwork() const
{
    Q_D(const SocialContentModel);
    return d->m_socialNetwork;
}

void SocialContentModel::setSocialNetwork(SocialNetwork *socialNetwork)
{
    Q_D(SocialContentModel);
    if (d->m_socialNetwork != socialNetwork) {
        d->m_socialNetwork = socialNetwork;
        emit socialNetworkChanged();
    }
}

SocialRequest * SocialContentModel::request() const
{
    Q_D(const SocialContentModel);
    return d->m_request;
}

void SocialContentModel::setRequest(SocialRequest *request)
{
    Q_D(SocialContentModel);
    if (d->m_request != request) {
        d->m_request = request;
        emit requestChanged();
    }
}

SocialContentModelBuilder * SocialContentModel::builder() const
{
    Q_D(const SocialContentModel);
    return d->m_builder;
}

void SocialContentModel::setBuilder(SocialContentModelBuilder *replyParser)
{
    Q_D(SocialContentModel);
    if (d->m_builder != replyParser) {
        d->m_builder = replyParser;
        emit builderChanged();
    }
}

SocialNetworkStatus::type SocialContentModel::status() const
{
    Q_D(const SocialContentModel);
    return d->status;
}

SocialNetworkError::type SocialContentModel::error() const
{
    Q_D(const SocialContentModel);
    return d->error;
}

QString SocialContentModel::errorString() const
{
    Q_D(const SocialContentModel);
    return d->errorString;
}

bool SocialContentModel::load()
{
    Q_D(SocialContentModel);
    return d->load(SocialContentModelPrivate::Replace);
}

bool SocialContentModel::loadNext()
{
    Q_D(SocialContentModel);
    return d->load(SocialContentModelPrivate::Append);
}

bool SocialContentModel::loadPrevious()
{
    Q_D(SocialContentModel);
    return d->load(SocialContentModelPrivate::Prepend);
}


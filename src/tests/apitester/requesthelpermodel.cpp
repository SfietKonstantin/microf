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

#include "requesthelpermodel.h"
#include <socialrequest.h>
#include <facebook/facebookfriendlistrequest.h>

class RequestHelperModelData
{
public:
    explicit RequestHelperModelData(const QString &name, SocialRequest *request,
                                    RequestHelperModel::Type type);
    QString name;
    SocialRequest *request;
    RequestHelperModel::Type type;
};

RequestHelperModelData::RequestHelperModelData(const QString &name, SocialRequest *request,
                                               RequestHelperModel::Type type)
    : name(name), request(request), type(type)
{
}

RequestHelperModel::RequestHelperModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QList<RequestHelperModelData *> data;
    data.append(new RequestHelperModelData("Friends", new FacebookFriendListRequest(this), Model));
    beginInsertRows(QModelIndex(), 0, data.count());
    m_data = data;
    emit countChanged();
    endInsertRows();
}

RequestHelperModel::~RequestHelperModel()
{
    qDeleteAll(m_data);
}

QHash<int, QByteArray> RequestHelperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(TextRole, "text");
    roles.insert(RequestRole, "request");
    roles.insert(TypeRole, "type");
    return roles;
}

int RequestHelperModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant RequestHelperModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    const RequestHelperModelData *data = m_data.at(row);
    switch (role) {
    case TextRole:
        return data->name;
        break;
    case RequestRole:
        return QVariant::fromValue(data->request);
        break;
    case TypeRole:
        return data->type;
        break;
    default:
        return QVariant();
        break;
    }
}

int RequestHelperModel::count() const
{
    return m_data.count();
}

SocialRequest * RequestHelperModel::request(int index) const
{
    if (index < 0 || index >= m_data.count()) {
        return nullptr;
    }
    return m_data.at(index)->request;
}

RequestHelperModel::Type RequestHelperModel::type(int index) const
{
    if (index < 0 || index >= m_data.count()) {
        return Invalid;
    }
    return m_data.at(index)->type;
}


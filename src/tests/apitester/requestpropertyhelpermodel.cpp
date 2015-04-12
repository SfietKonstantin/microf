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

#include "requestpropertyhelpermodel.h"
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

class RequestPropertyHelperModelData
{
public:
    QString name;
    RequestPropertyHelperModel::Type type;
    int propertyIndex;
};

RequestPropertyHelperModel::RequestPropertyHelperModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

RequestPropertyHelperModel::~RequestPropertyHelperModel()
{
    qDeleteAll(m_data);
}

QHash<int, QByteArray> RequestPropertyHelperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(TypeRole, "type");
    roles.insert(ValueRole, "value");
    return roles;
}

int RequestPropertyHelperModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant RequestPropertyHelperModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    const RequestPropertyHelperModelData *data = m_data.at(row);
    switch (role) {
    case NameRole:
        return data->name;
        break;
    case TypeRole:
        return data->type;
        break;
    case ValueRole:
    {
        const QMetaObject *meta = m_request->metaObject();
        const QMetaProperty &metaProperty = meta->property(data->propertyIndex);
        return metaProperty.read(m_request);
        break;
    }
    default:
        return QVariant();
        break;
    }
}

int RequestPropertyHelperModel::count() const
{
    return m_data.count();
}

QObject * RequestPropertyHelperModel::request() const
{
    return m_request;
}

void RequestPropertyHelperModel::setRequest(QObject *request)
{
    if (m_request != request) {
        m_request = request;
        emit requestChanged();
        updateMetaObject();
    }
}

void RequestPropertyHelperModel::save(int index, const QVariant &value)
{
    if (index < 0 || index >= m_data.count()) {
        return;
    }

    const RequestPropertyHelperModelData *data = m_data.at(index);

    const QMetaObject *meta = m_request->metaObject();
    const QMetaProperty &metaProperty = meta->property(data->propertyIndex);
    metaProperty.write(m_request, value);
    emit dataChanged(this->index(index), this->index(index));
}

void RequestPropertyHelperModel::clear(bool emitSignal)
{
    if (rowCount() > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        qDeleteAll(m_data);
        m_data.clear();
        if (emitSignal) {
            emit countChanged();
        }
        endRemoveRows();
    }
}

void RequestPropertyHelperModel::updateMetaObject()
{
    if (!m_request) {
        clear(true);
        return;
    }

    QList<RequestPropertyHelperModelData *> data;

    const QMetaObject *meta = m_request->metaObject();
    int offset = meta->propertyOffset();
    int count = meta->propertyCount();

    for (int i = offset; i < count; ++i) {
        const QMetaProperty &metaProperty = meta->property(i);
        RequestPropertyHelperModelData *entry = new RequestPropertyHelperModelData;
        entry->name = metaProperty.name();
        entry->propertyIndex = i;
        QMetaType::Type type = static_cast<QMetaType::Type>(metaProperty.type());
        switch (type) {
        case QMetaType::Int:
        case QMetaType::Long:
        case QMetaType::LongLong:
        case QMetaType::UInt:
        case QMetaType::ULong:
        case QMetaType::ULongLong:
            entry->type = Int;
            break;
        case QMetaType::Float:
        case QMetaType::Double:
            entry->type = Double;
            break;
        case QMetaType::QString:
            entry->type = String;
            break;
        default:
            entry->type = Unknown;
            break;
        }
        data.append(entry);
    }

    clear(data.isEmpty());

    if (data.count() > 0) {
        beginInsertRows(QModelIndex(), 0, data.count() - 1);
        m_data = data;
        emit countChanged();
        endInsertRows();
    }
}

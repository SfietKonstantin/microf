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

#include "buildershelpermodel.h"
#include "facebook/abstractfacebookmodelbuilder.h"
#include "facebook/facebookmodelbuilder.h"
#include "customfacebookmodelbuilder.h"

class BuildersHelperModelData
{
public:
    explicit BuildersHelperModelData(const QString &name, AbstractFacebookModelBuilder *builder);
    QString name;
    AbstractFacebookModelBuilder *builder;
};

BuildersHelperModelData::BuildersHelperModelData(const QString &name,
                                                 AbstractFacebookModelBuilder *builder)
    : name(name), builder(builder)
{
}

BuildersHelperModel::BuildersHelperModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QList<BuildersHelperModelData *> data;
    FacebookModelBuilder *standard = new FacebookModelBuilder(this);
    standard->setIncludeRawData(true);
    data.append(new BuildersHelperModelData("Standard", standard));
    CustomFacebookModelBuilder *custom = new CustomFacebookModelBuilder(this);
    custom->setIncludeRawData(true);
    data.append(new BuildersHelperModelData("Custom", custom));
    beginInsertRows(QModelIndex(), 0, data.count());
    m_data = data;
    emit countChanged();
    endInsertRows();
}

BuildersHelperModel::~BuildersHelperModel()
{
    qDeleteAll(m_data);
}

QHash<int, QByteArray> BuildersHelperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(TextRole, "text");
    roles.insert(BuilderRole, "builder");
    return roles;
}

int BuildersHelperModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant BuildersHelperModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    const BuildersHelperModelData *data = m_data.at(row);
    switch (role) {
    case TextRole:
        return data->name;
        break;
    case BuilderRole:
        return QVariant::fromValue(data->builder);
        break;
    default:
        return QVariant();
        break;
    }
}

int BuildersHelperModel::count() const
{
    return m_data.count();
}

AbstractFacebookModelBuilder *BuildersHelperModel::builder(int index) const
{
    if (index < 0 || index >= m_data.count()) {
        return nullptr;
    }
    return m_data.at(index)->builder;
}



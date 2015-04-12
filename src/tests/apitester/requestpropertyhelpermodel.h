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

#ifndef REQUESTPROPERTYHELPERMODEL_H
#define REQUESTPROPERTYHELPERMODEL_H

#include <QtCore/QAbstractListModel>

class SocialRequest;
class RequestPropertyHelperModelData;
class RequestPropertyHelperModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QObject * request READ request WRITE setRequest NOTIFY requestChanged)
    Q_ENUMS(Type)
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ValueRole
    };
    enum Type {
        Unknown,
        Int,
        Double,
        String
    };
    explicit RequestPropertyHelperModel(QObject *parent = 0);
    virtual ~RequestPropertyHelperModel();
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int count() const;
    QObject * request() const;
    void setRequest(QObject *request);
public slots:
    void save(int index, const QVariant &value);
signals:
    void countChanged();
    void requestChanged();
private:
    void clear(bool emitSignal);
    void updateMetaObject();
    QList<RequestPropertyHelperModelData *> m_data;
    QObject * m_request;
};



#endif // REQUESTPROPERTYHELPERMODEL_H

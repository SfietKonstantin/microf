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

#ifndef JSONTREEMODEL_H
#define JSONTREEMODEL_H

#include <QAbstractListModel>
#include <QJsonValue>
#include <memory>

class JsonTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Roles {
        KeyRole = Qt::UserRole + 1,
        LabelRole
    };
    explicit JsonTreeModel(QObject *parent = nullptr);
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void setValue(QJsonValue &&value);
private:
    class Node
    {
    public:
        explicit Node() = default;
        Node *parent {nullptr};
        int row {-1};
        QString key {};
        QString label {};
        QJsonValue value {};
        std::vector<Node> children {};
    };
    QHash<int, QByteArray> roleNames() const override final;
    void setProperties(Node &node, int row, QString &&key, QJsonValue &&value, Node *parent);
    void addChildren(QModelIndex &&parent, Node &node);
    void addChildrenForObject(QModelIndex &&parent, Node &node, QJsonObject &&object);
    void addChildrenForArray(QModelIndex &&parent, Node &node, QJsonArray &&array);
    std::unique_ptr<Node> m_root {};
};

#endif // JSONTREEMODEL_H

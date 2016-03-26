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

#include "jsontreemodel.h"
#include <QJsonArray>
#include <QJsonObject>

JsonTreeModel::JsonTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QModelIndex JsonTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (column < 0 || column >= 1) {
        return QModelIndex();
    }

    Node *parentNode {nullptr};

    if (!parent.isValid()) {
        parentNode = m_root.get();
    } else {
        parentNode = static_cast<Node *>(parent.internalPointer());
    }

    if (row < 0 || row >= static_cast<int>(parentNode->children.size())) {
        return QModelIndex();
    }

    Node &childItem = parentNode->children.at(row);
    return createIndex(row, column, &childItem);
}

QModelIndex JsonTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    Node *childNode = static_cast<Node *>(index.internalPointer());
    Node *parentNode= childNode->parent;

    if (parentNode == m_root.get()) {
        return QModelIndex();
    }

    return createIndex(parentNode->row, 0, parentNode);
}

int JsonTreeModel::rowCount(const QModelIndex &parent) const
{
    Node *parentNode {nullptr};
    if (!parent.isValid()) {
        parentNode = m_root.get();
    } else {
        parentNode = static_cast<Node *>(parent.internalPointer());
    }

    if (parentNode == nullptr) {
        return 0;
    }
    return parentNode->children.size();
}

int JsonTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant JsonTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.column() < 0 || index.column() >= 1) {
        return QModelIndex();
    }

    Node *node = static_cast<Node *>(index.internalPointer());

    switch (role) {
    case KeyRole:
        return node->key;
    case LabelRole:
        return node->label;
    default:
        return QVariant();
    }
}

void JsonTreeModel::setValue(QJsonValue &&value)
{
    if (m_root) {
        beginRemoveRows(QModelIndex(), 0, m_root->children.size() - 1);
        m_root.reset(new Node());
        endRemoveRows();
    } else {
        m_root.reset(new Node());
    }
    setProperties(*m_root, -1, QLatin1String("root"), std::move(value), nullptr);
    addChildren(QModelIndex(), *m_root);
}

QHash<int, QByteArray> JsonTreeModel::roleNames() const
{
    return {
        {KeyRole, "key"},
        {LabelRole, "label"}
    };
}

void JsonTreeModel::setProperties(Node &node, int row, QString &&key, QJsonValue &&value, Node *parent)
{
    node.row = row;
    node.key = std::move(key);
    node.value = std::move(value);
    node.parent = parent;
    switch (node.value.type()) {
    case QJsonValue::Null:
        node.label = "null";
        break;
    case QJsonValue::Bool:
        node.label = node.value.toBool() ? QLatin1String("true") : QLatin1String("false");
        break;
    case QJsonValue::Double:
        node.label = QString::number(node.value.toDouble());
        break;
    case QJsonValue::String:
        node.label = node.value.toString();
        break;
    case QJsonValue::Array:
        node.label = QLatin1String("[array]");
        break;
    case QJsonValue::Object:
        node.label = QLatin1String("{object}");
        break;
    case QJsonValue::Undefined:
        break;
    }
}

void JsonTreeModel::addChildren(QModelIndex &&parent, Node &node)
{
    if (node.value.isObject()) {
        addChildrenForObject(std::move(parent), node, node.value.toObject());
    } else if (node.value.isArray()) {
        addChildrenForArray(std::move(parent), node, node.value.toArray());
    }
}

void JsonTreeModel::addChildrenForObject(QModelIndex &&parent, JsonTreeModel::Node &node,
                                         QJsonObject &&object)
{
    int count {object.count()};
    beginInsertRows(parent, 0, count - 1);
    node.children.reserve(count);

    int row {0};
    for (auto it = object.begin(); it != object.end(); ++it) {
        node.children.emplace_back();
        Node &child = node.children.back();
        setProperties(child, row, it.key(), it.value(), &node);
        ++row;
    }

    endInsertRows();

    for (Node &child : node.children) {
        addChildren(index(child.row, 0, parent), child);
    }
}

void JsonTreeModel::addChildrenForArray(QModelIndex &&parent, JsonTreeModel::Node &node,
                                        QJsonArray &&array)
{
    int count {array.count()};
    beginInsertRows(parent, 0, count - 1);
    node.children.reserve(count);

    int row {0};
    for (int i = 0; i < count; ++i) {
        node.children.emplace_back();
        Node &child = node.children.back();
        setProperties(child, row, QString::number(i), array.at(i), &node);
        ++row;
    }

    endInsertRows();

    for (Node &child : node.children) {
        addChildren(index(child.row, 0, parent), child);
    }
}

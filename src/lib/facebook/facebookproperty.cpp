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

#include "facebookproperty.h"
#include "facebookproperty_p.h"
#include <QtCore/QStringList>
#include <QtCore/QVariant>

FacebookPropertyPrivate::FacebookPropertyPrivate(FacebookProperty *q)
    : q_ptr(q)
{
}

QJsonValue FacebookPropertyPrivate::valueFromPath(const QJsonObject &object, const QString &path)
{
    QStringList splittedPath = path.split("/");
    QJsonValue currentValue = QJsonValue(object);
    while (!splittedPath.isEmpty()) {
        const QString &step = splittedPath.takeFirst();
        currentValue = currentValue.toObject().value(step);
    }
    return currentValue;
}

QJsonArray FacebookPropertyPrivate::arrayFromPath(const QJsonObject &object, const QString &path)
{
    const QJsonValue &currentValue = valueFromPath(object, path);
    if (!currentValue.isArray()) {
        return QJsonArray();
    }
    return currentValue.toArray();
}

QVariant FacebookPropertyPrivate::propertyFromPath(const QJsonObject &object, const QString &path)
{
    const QJsonValue &currentValue = valueFromPath(object, path);
    if (currentValue.isArray()) {
        const QJsonArray &currentValueArray = currentValue.toArray();
        if (!isCorrectArray(currentValueArray)) {
            return QVariant();
        } else {
            return currentValueArray;
        }
    }
    if (!currentValue.isBool() && !currentValue.isDouble() && !currentValue.isString()) {
        return QVariant();
    }
    return currentValue.toVariant();
}

bool FacebookPropertyPrivate::isCorrectArray(const QJsonArray &array)
{
    bool ok = true;
    for (const QJsonValue &value : array) {
        if (value.isObject() || value.isUndefined()) {
            ok = false;
            break;
        }
    }
    return ok;
}

FacebookProperty::FacebookProperty(QObject *parent)
    : QObject(parent), d_ptr(new FacebookPropertyPrivate(this))
{
}

FacebookProperty::FacebookProperty(FacebookPropertyPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

FacebookProperty::~FacebookProperty()
{
}

void FacebookProperty::classBegin()
{
}

void FacebookProperty::componentComplete()
{
}

QString FacebookProperty::path() const
{
    Q_D(const FacebookProperty);
    return d->path;
}

void FacebookProperty::setPath(const QString &path)
{
    Q_D(FacebookProperty);
    if (d->path != path) {
        d->path = path;
        emit pathChanged();
    }
}

QString FacebookProperty::name() const
{
    Q_D(const FacebookProperty);
    return d->name;
}

void FacebookProperty::setName(const QString &name)
{
    Q_D(FacebookProperty);
    if (d->name != name) {
        d->name = name;
        emit nameChanged();
    }
}


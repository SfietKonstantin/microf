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

#include "abstractfacebookmodelbuilder.h"
#include "abstractfacebookmodelbuilder_p.h"
#include <QtCore/QJsonDocument>

AbstractFacebookModelBuilderPrivate::AbstractFacebookModelBuilderPrivate(AbstractFacebookModelBuilder *q)
    : SocialContentModelBuilderPrivate(q)
{
}

void AbstractFacebookModelBuilderPrivate::properties_append(QQmlListProperty<FacebookProperty> *list,
                                                            FacebookProperty *property)
{
    AbstractFacebookModelBuilder *object = qobject_cast<AbstractFacebookModelBuilder *>(list->object);
    if (!object || !property) {
        return;
    }
    object->d_func()->m_properties.append(property);
}

int AbstractFacebookModelBuilderPrivate::properties_count(QQmlListProperty<FacebookProperty> *list)
{
    AbstractFacebookModelBuilder *object = qobject_cast<AbstractFacebookModelBuilder *>(list->object);
    if (!object) {
        return 0;
    }
    return object->d_func()->m_properties.count();
}

FacebookProperty * AbstractFacebookModelBuilderPrivate::properties_at(QQmlListProperty<FacebookProperty> *list,
                                                                      int index)
{
    AbstractFacebookModelBuilder *object = qobject_cast<AbstractFacebookModelBuilder *>(list->object);
    if (!object) {
        return nullptr;
    }

    const QList<FacebookProperty *> &properties = object->d_func()->m_properties;
    if (index < 0 || index >= properties.count()) {
        return nullptr;
    }

    return properties.at(index);
}

void AbstractFacebookModelBuilderPrivate::properties_clear(QQmlListProperty<FacebookProperty> *list)
{
    AbstractFacebookModelBuilder *object = qobject_cast<AbstractFacebookModelBuilder *>(list->object);
    if (!object) {
        return;
    }

    object->d_func()->clear();
}

const QList<FacebookProperty *> AbstractFacebookModelBuilderPrivate::properties() const
{
    return m_properties;
}

void AbstractFacebookModelBuilderPrivate::clear()
{
    qDeleteAll(m_properties);
    m_properties.clear();
}

AbstractFacebookModelBuilder::AbstractFacebookModelBuilder(AbstractFacebookModelBuilderPrivate &dd, QObject *parent)
    : SocialContentModelBuilder(dd, parent)
{
}

AbstractFacebookModelBuilder::~AbstractFacebookModelBuilder()
{
    Q_D(AbstractFacebookModelBuilder);
    d->clear();
}

QQmlListProperty<FacebookProperty> AbstractFacebookModelBuilder::properties()
{
    return QQmlListProperty<FacebookProperty>(this, 0,
                                              &AbstractFacebookModelBuilderPrivate::properties_append,
                                              &AbstractFacebookModelBuilderPrivate::properties_count,
                                              &AbstractFacebookModelBuilderPrivate::properties_at,
                                              &AbstractFacebookModelBuilderPrivate::properties_clear);
}

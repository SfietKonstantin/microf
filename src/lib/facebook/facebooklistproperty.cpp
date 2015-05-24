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

#include "facebooklistproperty.h"
#include "facebookproperty_p.h"

class FacebookListPropertyPrivate: public FacebookPropertyPrivate
{
public:
    explicit FacebookListPropertyPrivate(FacebookListProperty *q);
    static void properties_append(QQmlListProperty<FacebookProperty> *list, FacebookProperty *property);
    static int properties_count(QQmlListProperty<FacebookProperty> *list);
    static FacebookProperty * properties_at(QQmlListProperty<FacebookProperty> *list, int index);
    static void properties_clear(QQmlListProperty<FacebookProperty> *list);
private:
    Q_DECLARE_PUBLIC(FacebookListProperty)
    void clear();
    QList<FacebookProperty *> m_properties;
};

FacebookListPropertyPrivate::FacebookListPropertyPrivate(FacebookListProperty *q)
    : FacebookPropertyPrivate(q)
{
}

void FacebookListPropertyPrivate::properties_append(QQmlListProperty<FacebookProperty> *list,
                                                    FacebookProperty *property)
{
    FacebookListProperty *object = qobject_cast<FacebookListProperty *>(list->object);
    if (!object || !property) {
        return;
    }
    object->d_func()->m_properties.append(property);
}

int FacebookListPropertyPrivate::properties_count(QQmlListProperty<FacebookProperty> *list)
{
    FacebookListProperty *object = qobject_cast<FacebookListProperty *>(list->object);
    if (!object) {
        return 0;
    }
    return object->d_func()->m_properties.count();
}

FacebookProperty * FacebookListPropertyPrivate::properties_at(QQmlListProperty<FacebookProperty> *list,
                                                              int index)
{
    FacebookListProperty *object = qobject_cast<FacebookListProperty *>(list->object);
    if (!object) {
        return nullptr;
    }

    const QList<FacebookProperty *> &properties = object->d_func()->m_properties;
    if (index < 0 || index >= properties.count()) {
        return nullptr;
    }

    return properties.at(index);
}

void FacebookListPropertyPrivate::properties_clear(QQmlListProperty<FacebookProperty> *list)
{
    FacebookListProperty *object = qobject_cast<FacebookListProperty *>(list->object);
    if (!object) {
        return;
    }

    object->d_func()->clear();
}

void FacebookListPropertyPrivate::clear()
{
    qDeleteAll(m_properties);
    m_properties.clear();
}

FacebookListProperty::FacebookListProperty(QObject *parent)
    : FacebookProperty(*(new FacebookListPropertyPrivate(this)), parent)
{
}

FacebookListProperty::~FacebookListProperty()
{
    Q_D(FacebookListProperty);
    d->clear();
}

QQmlListProperty<FacebookProperty> FacebookListProperty::properties()
{
    return QQmlListProperty<FacebookProperty>(this, 0,
                                              &FacebookListPropertyPrivate::properties_append,
                                              &FacebookListPropertyPrivate::properties_count,
                                              &FacebookListPropertyPrivate::properties_at,
                                              &FacebookListPropertyPrivate::properties_clear);
}

const QList<FacebookProperty *> & FacebookListProperty::propertiesList() const
{
    Q_D(const FacebookListProperty);
    return d->m_properties;
}


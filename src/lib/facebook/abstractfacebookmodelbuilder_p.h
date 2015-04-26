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

#ifndef ABSTRACTFACEBOOKMODELBUILDER_P_H
#define ABSTRACTFACEBOOKMODELBUILDER_P_H

#include "socialcontentmodelbuilder_p.h"
#include "abstractfacebookmodelbuilder.h"

class AbstractFacebookModelBuilderPrivate: public SocialContentModelBuilderPrivate
{
public:
    explicit AbstractFacebookModelBuilderPrivate(AbstractFacebookModelBuilder *q);
    static void properties_append(QQmlListProperty<FacebookProperty> *list, FacebookProperty *property);
    static int properties_count(QQmlListProperty<FacebookProperty> *list);
    static FacebookProperty * properties_at(QQmlListProperty<FacebookProperty> *list, int index);
    static void properties_clear(QQmlListProperty<FacebookProperty> *list);
    void setRawData(const QString &rawData);
    const QList<FacebookProperty *> properties() const;
    void writeRawData(const QByteArray &data);
protected:
    QList<FacebookProperty *> m_properties;
private:
    void clear();
    bool m_includeRawData;
    QString m_rawData;
    Q_DECLARE_PUBLIC(AbstractFacebookModelBuilder)
};


#endif // ABSTRACTFACEBOOKMODELBUILDER_P_H


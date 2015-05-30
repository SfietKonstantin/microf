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

#ifndef INFOHELPER_H
#define INFOHELPER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>

class InfoHelper: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap object READ object WRITE setObject NOTIFY objectChanged)
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    Q_PROPERTY(QStringList urls READ urls NOTIFY urlsChanged)
public:
    explicit InfoHelper(QObject *parent = 0);
    ~InfoHelper();
    const QVariantMap & object() const;
    void setObject(const QVariantMap &object);
    QString text() const;
    QStringList urls() const;
public slots:
    void refresh();
signals:
    void objectChanged();
    void textChanged();
    void urlsChanged();
private:
    void generateText();
    QVariantMap m_object;
    QString m_text;
    QStringList m_urls;
};

#endif // INFOHELPER_H

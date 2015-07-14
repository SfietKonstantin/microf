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

#ifndef PROXYBUILDERHELPER_H
#define PROXYBUILDERHELPER_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QTextStream>

static inline void recursiveGetSummary(const QString &key, const QJsonValue &value,
                                       QTextStream &stream)
{
    stream << "<li>"
           << "<b>" << key << "</b>"
           << " ";
    if (value.isArray()) {
        stream << "(array)";
    } else if (value.isBool()) {
        stream << "(bool) : " << (value.toBool() ? "true" : "false");
    } else if (value.isDouble()) {
        stream << "(double) : " << value.toDouble();
    } else if (value.isNull()) {
        stream << "(null)";
    } else if (value.isObject()) {
        stream << "(object)";
    } else if (value.isString()) {
        stream << "(string) : " << value.toString();
    }

    if (value.isArray()) {
        stream << "<ul>";
        const QJsonArray &array = value.toArray();
        for (int i = 0; i < array.size(); ++i) {
            recursiveGetSummary(QString::number(i), array.at(i), stream);
        }
        stream << "</ul>";
    } else if (value.isObject()) {
        stream << "<ul>";
        const QJsonObject &object = value.toObject();
        for (const QString &key : object.keys()) {
            recursiveGetSummary(key, object.value(key), stream);
        }
        stream << "</ul>";
    }
    stream << "</li>";
}

static inline void getSummary(const QJsonDocument &document, QTextStream &stream)
{
    if (document.isObject()) {
        recursiveGetSummary("ROOT", QJsonValue(document.object()), stream);
    } else if (document.isArray()) {
        const QJsonArray &array = document.array();
        for (int i = 0; i < array.size(); ++i) {
            recursiveGetSummary(QString::number(i), array.at(i), stream);
        }
    }
}

#endif // PROXYBUILDERHELPER_H


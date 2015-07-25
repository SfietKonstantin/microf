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

#ifndef SEARCHHELPER_H
#define SEARCHHELPER_H

#include <QtCore/QPointer>
#include <QtGui/QSyntaxHighlighter>
#include <QtQuick/QQuickTextDocument>

class SearchHelper: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickTextDocument * textDocument READ textDocument WRITE setTextDocument
               NOTIFY textDocumentChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    explicit SearchHelper(QObject *parent = 0);
    ~SearchHelper();
    QQuickTextDocument * textDocument() const;
    void setTextDocument(QQuickTextDocument *textDocument);
    QString text() const;
    void setText(const QString &text);
public slots:
    void next();
signals:
    void textDocumentChanged();
    void textChanged();
    void selectionChanged(int begin, int end);
private:
    class Highlighter: public QSyntaxHighlighter
    {
    public:
        explicit Highlighter(const SearchHelper &helper, QTextDocument *parent);
    protected:
        void highlightBlock(const QString &text) override;
    private:
        const SearchHelper &m_helper;
    };
    QQuickTextDocument *m_textDocument;
    QPointer<Highlighter> m_highlighter;
    QString m_text;
    QTextCursor m_cursor;
private slots:
    void slotCursorChanged(const QTextCursor &cursor);
};

#endif // SEARCHHELPER_H

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

#include "searchhelper.h"
#include <QtGui/QTextCursor>

SearchHelper::SearchHelper(QObject *parent)
    : QObject(parent), m_textDocument(0), m_highlighter(0)
{
}

SearchHelper::~SearchHelper()
{
    m_highlighter->deleteLater();
}

QQuickTextDocument * SearchHelper::textDocument() const
{
    return m_textDocument;
}

void SearchHelper::setTextDocument(QQuickTextDocument *textDocument)
{
    if (m_textDocument != textDocument) {
        if (m_textDocument) {
            disconnect(m_textDocument->textDocument(), 0, this, 0);
        }
        m_textDocument = textDocument;
        if (m_textDocument) {
            m_highlighter = QPointer<Highlighter>(new Highlighter(*this, m_textDocument->textDocument()));
            connect(m_textDocument->textDocument(), &QTextDocument::cursorPositionChanged,
                    this, &SearchHelper::slotCursorChanged);
        }
        emit textDocumentChanged();
    }
}

QString SearchHelper::text() const
{
    return m_text;
}

void SearchHelper::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged();

        if (m_highlighter) {
            // Force highlighting (in a very ugly way)
            m_highlighter->setDocument(0);
            m_highlighter->setDocument(m_textDocument->textDocument());
        }
    }
}

void SearchHelper::next()
{
    m_cursor = m_textDocument->textDocument()->find(m_text, m_cursor);
    if (m_cursor.isNull()) {
        m_cursor = m_textDocument->textDocument()->find(m_text, m_cursor);
    }
    if (!m_cursor.isNull()) {
        emit selectionChanged(m_cursor.selectionStart(), m_cursor.selectionEnd());
    }
}

void SearchHelper::slotCursorChanged(const QTextCursor &cursor)
{
    m_cursor = cursor;
}

SearchHelper::Highlighter::Highlighter(const SearchHelper &helper, QTextDocument *parent)
    : QSyntaxHighlighter(parent), m_helper(helper)
{
}

void SearchHelper::Highlighter::highlightBlock(const QString &text)
{
    if (m_helper.m_text.isEmpty()) {
        return;
    }

    QTextCharFormat format;
    format.setBackground(QBrush(QColor(Qt::yellow)));
    format.setForeground(QBrush(QColor(Qt::red)));

    int count = m_helper.m_text.count();
    int index = text.indexOf(m_helper.m_text, 0, Qt::CaseInsensitive);
    while (index != -1) {
        setFormat(index, count, format);
        index = text.indexOf(m_helper.m_text, index + count, Qt::CaseInsensitive);
    }
}


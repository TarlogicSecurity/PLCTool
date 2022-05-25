//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "PLCTool/Ui/XMLHighlighter.h"

XMLHighlighter::XMLHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
  HighlightingRule rule;

  // >text<
  QTextCharFormat xmlValueElementFormat;
  xmlValueElementFormat.setForeground(Qt::black);
  xmlValueElementFormat.setFontWeight(QFont::Bold);
  rule.pattern = QRegExp(">[^\n]*<");
  rule.format = xmlValueElementFormat;
  highlightingRules.append(rule);

  // keywords
  QTextCharFormat keywordFormat;
  keywordFormat.setForeground(Qt::blue);
  keywordFormat.setFontWeight(QFont::Bold);
  QStringList keywords;
  keywords << "\\b?xml\\b"
           << "/>"
           << ">"
           << "<";
  foreach (const QString &keyword, keywords) {
    rule.pattern = QRegExp(keyword);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
  }

  // <Text> </Text>
  QTextCharFormat xmlElementFormat;
  xmlElementFormat.setForeground(Qt::blue);
  rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=[\\s\\/>])");
  rule.format = xmlElementFormat;
  highlightingRules.append(rule);

  // < Text= >
  QTextCharFormat xmlAttributeFormat;
  xmlAttributeFormat.setForeground(Qt::red);
  rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\=)");
  rule.format = xmlAttributeFormat;
  highlightingRules.append(rule);

  // <!-- Text -->
  QTextCharFormat singleLineCommentFormat;
  singleLineCommentFormat.setForeground(Qt::gray);
  rule.pattern = QRegExp("<!--[^\n]*-->");
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);

  // = "Text"
  QColor valueColor(0, 0, 0);
  valueFormat.setForeground(valueColor);
  valueFormat.setFontWeight(QFont::Bold);
  valueStartExpression.setPattern("\"");
  valueEndExpression.setPattern("\"(?=[\\s></])");
}

XMLHighlighter::~XMLHighlighter()
{
}

void
XMLHighlighter::highlightBlock(const QString &text)
{
  // for every pattern
  foreach (const HighlightingRule &rule, highlightingRules) {
    QRegExp expression(rule.pattern);
    int index = expression.indexIn(text);

    while (index >= 0) {
      int length = expression.matchedLength();
      this->setFormat(index, length, rule.format);
      index = expression.indexIn(text, index + length);
    }
  }

  this->setCurrentBlockState(0);
  int startIndex = 0;

  if (this->previousBlockState() != 1)
    startIndex = valueStartExpression.indexIn(text);

  while (startIndex >= 0) {
    int endIndex = valueEndExpression.indexIn(text, startIndex);
    int commentLength;

    if (endIndex == -1) {
      this->setCurrentBlockState(1);
      commentLength = text.length() - startIndex;
    } else {
      commentLength =
          endIndex - startIndex + valueEndExpression.matchedLength();
    }

    this->setFormat(startIndex, commentLength, valueFormat);
    startIndex = valueStartExpression.indexIn(text, startIndex + commentLength);
  }
}

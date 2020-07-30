#include "Ui/XMLHighlighter.h"


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
  keywords << "\\b?xml\\b" << "/>" << ">" << "<";
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
XMLHighlighter::highlightBlock(const QString & text)
{
  //for every pattern
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
      commentLength = endIndex - startIndex + valueEndExpression.matchedLength();
    }

    this->setFormat(startIndex, commentLength, valueFormat);
    startIndex = valueStartExpression.indexIn(text, startIndex + commentLength);
  }
}

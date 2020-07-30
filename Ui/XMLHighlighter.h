#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class XMLHighlighter : public QSyntaxHighlighter
{
  struct HighlightingRule
  {
      QRegExp pattern;
      QTextCharFormat format;
  };

  QVector<HighlightingRule> highlightingRules;

  QTextCharFormat valueFormat;
  QRegExp valueStartExpression;
  QRegExp valueEndExpression;

  void highlightBlock(const QString & text);

public:
  XMLHighlighter(QTextDocument *parent);
  virtual ~XMLHighlighter();

};

#endif // XMLHIGHLIGHTER_H

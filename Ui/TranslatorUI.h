#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QWidget>

namespace Ui {
  class Translator;
}

class XMLHighlighter;

class TranslatorUI : public QWidget
{
  Q_OBJECT

  XMLHighlighter *highlighter = nullptr;
  void connectAll(void);

public:
  explicit TranslatorUI(QWidget *parent = 0);
  ~TranslatorUI();

public slots:
  void onPduToXml(void);

private:
  Ui::Translator *ui;
};

#endif // TRANSLATOR_H

#include "Ui/TranslatorUI.h"
#include "ui_TranslatorUI.h"
#include <gurux/include/GXDLMSTranslator.h>
#include <QMessageBox>
#include "XMLHighlighter.h"

TranslatorUI::TranslatorUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Translator)
{
  ui->setupUi(this);

  this->highlighter = new XMLHighlighter(this->ui->xmlEdit->document());
  this->connectAll();
}

TranslatorUI::~TranslatorUI()
{
  delete ui;
}

void
TranslatorUI::connectAll(void)
{
  connect(
        this->ui->pduToXmlButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onPduToXml(void)));
}

///////////////////////////////// Slots ////////////////////////////////////////
void
TranslatorUI::onPduToXml(void)
{
  CGXDLMSTranslator translator(DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML);
  CGXByteBuffer buffer;
  std::string xml;

  buffer.SetHexString(this->ui->bytesEdit->toPlainText().toStdString());
  this->ui->bytesEdit->setPlainText(
        QString::fromStdString((buffer.ToHexString())));

  if (translator.PduToXml(buffer, xml) != 0) {
    this->ui->xmlEdit->setPlainText("");
    QMessageBox::critical(
          this,
          "Convert bytes to XML",
          "Failed to parse DLMS PDU.");

  } else {
    this->ui->xmlEdit->setPlainText(QString::fromStdString(xml));
  }
}


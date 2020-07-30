#ifndef DLMSLOGUI_H
#define DLMSLOGUI_H

#include <QWidget>
#include <GXDLMSTranslator.h>
#include <QVector>
#include <QDateTime>
#include <Topology/Concentrator.h>
#include <QModelIndex>

namespace Ui {
  class DLMSLogUI;
}

struct DlmsMessage {
  QDateTime timeStamp;
  QString SNA;
  QString nodeId;
  QString type;
  bool downlink;
  QVector<uint8_t> pdu;
  QString xml;

  void setDlmsData(const uint8_t *data, size_t size);
  QString toText(void) const;
};


class DLMSLogUI : public QWidget
{
  Q_OBJECT

  QString savedText;
  CGXDLMSTranslator translator;
  QVector<DlmsMessage> messageList;

  void colorizeRow(int row, const QColor &color);
  void saveLog(QString path);

  void saveMessage(const DlmsMessage &msg);
  void connectAll(void);

public:
  explicit DLMSLogUI(QWidget *parent = 0);
  void pushMessage(
      const PLCTool::Concentrator *,
      PLCTool::NodeId,
      bool downlink,
      const void *data,
      size_t size);

  ~DLMSLogUI();

public slots:
  void onCellActivated(int row, int column);
  void onCurrentChanged(QModelIndex, QModelIndex);
  void onSaveAs(bool);
  void onClear(bool);


private:
  Ui::DLMSLogUI *ui;
};

#endif // DLMSLOGUI_H

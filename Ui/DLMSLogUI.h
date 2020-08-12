#ifndef DLMSLOGUI_H
#define DLMSLOGUI_H

#include <QWidget>
#include <QVector>
#include <QDateTime>
#include <Topology/Concentrator.h>
#include <QModelIndex>
#include "DLMSTableModel.h"
#include <QThread>
#include <QSortFilterProxyModel>

namespace Ui {
  class DLMSLogUI;
}

class XMLHighlighter;

class DLMSLogUI : public QWidget
{
  Q_OBJECT

  DLMSProcessor *processor;
  QThread *procThread;

  DLMSTableModel *model = nullptr;
  QSortFilterProxyModel *proxy = nullptr;

  QVector<DlmsMessage> messageList;
  QString savedText;

  XMLHighlighter *highlighter;

  void saveLog(QString path);

  void saveMessage(const DlmsMessage &msg);
  void connectAll(void);
  void connectProcessor(void);

public:
  static void registerTypes(void);

  explicit DLMSLogUI(QWidget *parent = 0);
  void pushMessage(
      const PLCTool::Concentrator *,
      QDateTime timeStamp,
      PLCTool::NodeId,
      bool downlink,
      const void *data,
      size_t size);
  void refreshMessages(void);
  int findMessage(QDateTime const &);
  void selectNear(QDateTime const &, PLCTool::NodeId);
  void clear(void);

  ~DLMSLogUI();

signals:
  void messageSelected(DlmsMessage);
  void messageReceived(
      QString SNA,
      QDateTime timeStamp,
      quint64 id,
      bool downlink,
      QVector<uint8_t>);

public slots:
  void onCellActivated(QModelIndex const &);
  void onCurrentChanged(QModelIndex, QModelIndex);
  void onSaveAs(bool);
  void onClear(bool);

  void onTop(void);
  void onBottom(void);
  void onGotoLine(void);
  void onDlmsMessage(DlmsMessage);

private:
  Ui::DLMSLogUI *ui;
};

#endif // DLMSLOGUI_H

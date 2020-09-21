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

  DLMSTableModel *model = nullptr;
  QSortFilterProxyModel *proxy = nullptr;

  QVector<DlmsMessage> messageList;
  QVector<DlmsMessage> pendingList;
  QString savedText;

  XMLHighlighter *highlighter;
  bool adjusting = false;
  bool sortingEnabled = false;
  void saveLog(QString path);

  void connectAll(void);

public:
  static void registerTypes(void);

  explicit DLMSLogUI(QWidget *parent = 0);
  void refreshMessages(void);
  int findMessage(QDateTime const &);
  void selectNear(QDateTime const &, PLCTool::NodeId);
  void setSortingEnabled(bool);
  void clear(void);

  ~DLMSLogUI();

signals:
  void messageSelected(DlmsMessage);

public slots:
  void saveMessage(const DlmsMessage &msg);

  void onCellActivated(QModelIndex const &);
  void onCurrentChanged(QModelIndex, QModelIndex);
  void onSaveAs(bool);
  void onClear(bool);

  void onTop(void);
  void onBottom(void);
  void onGotoLine(void);

private:
  Ui::DLMSLogUI *ui;
};

#endif // DLMSLOGUI_H

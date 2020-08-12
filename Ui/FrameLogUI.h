#ifndef FRAMELOGUI_H
#define FRAMELOGUI_H

#include <QWidget>
#include <Topology/Concentrator.h>
#include <QModelIndex>
#include <QVector>
#include <QDateTime>
#include <PRIME/PrimeFrame.h>
#include <QSortFilterProxyModel>
#include <QColor>
#include "FrameTableModel.h"
#include <QThread>

namespace Ui {
  class FrameLogUI;
}

class QTableWidgetItem;

class FrameLogUI : public QWidget
{
  Q_OBJECT

  QThread *procThread = nullptr;
  PRIMEProcessor *processor = nullptr;

  FrameTableModel *model = nullptr;
  QSortFilterProxyModel *proxy = nullptr;

  QVector<Frame> frameList;
  QString savedHtml;

  void saveLog(QString path);
  void saveFrame(Frame const &);

  void connectProcessor(void);
  void connectAll(void);

public:
  static void registerTypes(void);

  explicit FrameLogUI(QWidget *parent = 0);
  void pushFrame(
      const PLCTool::Concentrator *,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);

  void clear(void);
  void refreshFrames(void);
  void selectNear(
      QDateTime const &,
      PLCTool::PrimeFrame::GenericType,
      PLCTool::NodeId);
  ~FrameLogUI();

signals:
  void frameSelected(Frame &);
  void frameReceived(
          quint64 dcId,
          QDateTime timeStamp,
          bool downlink,
          QVector<uint8_t>);

public slots:
  void onCellActivated(const QModelIndex &);
  void onCurrentChanged(QModelIndex, QModelIndex);
  void onSaveAs(bool);
  void onClear(bool);

  void onTop(void);
  void onBottom(void);
  void onGotoLine(void);
  void onFrame(Frame);

private:
  Ui::FrameLogUI *ui;
};

#endif // FRAMELOGUI_H

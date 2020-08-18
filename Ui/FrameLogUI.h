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

  FrameTableModel *model = nullptr;
  QSortFilterProxyModel *proxy = nullptr;

  QVector<Frame> frameList;
  QString savedHtml;

  void saveLog(QString path);

  void connectAll(void);
  bool adjusting = false;

public:
  static void registerTypes(void);

  explicit FrameLogUI(QWidget *parent = 0);
  void refreshFrames(void);
  void selectNear(
      QDateTime const &,
      PLCTool::PrimeFrame::GenericType,
      PLCTool::NodeId);
  void clear(void);

  ~FrameLogUI();

signals:
  void frameSelected(Frame &);

public slots:
  void saveFrame(Frame const &);

  void onCellActivated(const QModelIndex &);
  void onCurrentChanged(QModelIndex, QModelIndex);
  void onSaveAs(bool);
  void onClear(bool);

  void onTop(void);
  void onBottom(void);
  void onGotoLine(void);

private:
  Ui::FrameLogUI *ui;
};

#endif // FRAMELOGUI_H

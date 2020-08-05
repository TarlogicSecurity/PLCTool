#ifndef FRAMELOGUI_H
#define FRAMELOGUI_H

#include <QWidget>
#include <Topology/Concentrator.h>
#include <QModelIndex>
#include <QVector>
#include <QDateTime>
#include <PRIME/PrimeFrame.h>
#include <QColor>

namespace Ui {
  class FrameLogUI;
}

class QTableWidgetItem;

struct Frame {
  QString SNA;
  bool downlink;
  QVector<uint8_t> bytes;
  QDateTime timeStamp;
  PLCTool::PrimeFrame *frame = nullptr;

  Frame();
  Frame(const Frame &);
  QString toHtml(void) const;

  ~Frame();
};

class FrameLogUI : public QWidget
{
  Q_OBJECT

  QVector<Frame> frameList;
  QString savedHtml;

  void colorizeRow(int row, const QColor &color);
  void saveLog(QString path);
  int saveFrame(
      const PLCTool::Concentrator *,
      bool downlink,
      const void *data,
      size_t size,
      PLCTool::PrimeFrame *);

  void connectAll(void);

public:
  explicit FrameLogUI(QWidget *parent = 0);

  void pushFrame(
      const PLCTool::Concentrator *,
      bool downlink,
      const void *data,
      size_t size);

  ~FrameLogUI();

public slots:
  void onCellActivated(int row, int column);
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

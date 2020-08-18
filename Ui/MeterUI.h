#ifndef METERUI_H
#define METERUI_H

#include <QWidget>
#include <App/MeterInfo.h>
#include "FrameTableModel.h"
#include "DLMSTableModel.h"
#include <QSortFilterProxyModel>

namespace Ui {
class MeterUI;
}

class XMLHighlighter;
class MeterUI : public QWidget
{
  Q_OBJECT

  QString savedHtml;
  QString savedText;
  MeterInfo *info = nullptr;
  FrameTableModel *frameModel = nullptr;
  DLMSTableModel  *dlmsModel = nullptr;
  QSortFilterProxyModel *frameProxy = nullptr;
  QSortFilterProxyModel *dlmsProxy = nullptr;
  XMLHighlighter *highlighter = nullptr;

  void connectAll(void);
  void connectMeterInfo(void);

public:
  explicit MeterUI(QWidget *parent, MeterInfo *);

  inline const MeterInfo *
  meterInfo(void) const
  {
    return this->info;
  }

  void refreshViews(void);

  ~MeterUI();

public slots:
  void onFrame(Frame);
  void onDlmsMessage(DlmsMessage);
  void onCreds(QString, QString);

  void onFrameCellActivated(const QModelIndex &index);
  void onFrameCurrentChanged(QModelIndex, QModelIndex);

  void onDlmsCellActivated(const QModelIndex &index);
  void onDlmsCurrentChanged(QModelIndex, QModelIndex);

  void onCredentialsFound(QDateTime, QString, QString);

private:
    Ui::MeterUI *ui;
};

#endif // METERUI_H

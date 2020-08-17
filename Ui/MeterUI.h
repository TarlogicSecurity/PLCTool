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

class MeterUI : public QWidget
{
  Q_OBJECT

  MeterInfo *info = nullptr;
  FrameTableModel *frameModel = nullptr;
  DLMSTableModel  *dlmsModel = nullptr;
  QSortFilterProxyModel *frameProxy = nullptr;
  QSortFilterProxyModel *dlmsProxy = nullptr;

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

private:
    Ui::MeterUI *ui;
};

#endif // METERUI_H

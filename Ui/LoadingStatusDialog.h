#ifndef LOADINGSTATUSDIALOG_H
#define LOADINGSTATUSDIALOG_H

#include <QDialog>

namespace Ui {
  class LoadingStatusDialog;
}

class LoadingStatusDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoadingStatusDialog(QWidget *parent = 0);
  ~LoadingStatusDialog();

  void setStatus(QString);

private:
  Ui::LoadingStatusDialog *ui;
};

#endif // LOADINGSTATUSDIALOG_H

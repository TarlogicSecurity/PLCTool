#ifndef MODEMDIALOG_H
#define MODEMDIALOG_H

#include <QDialog>

namespace Ui {
class ModemDialog;
}

class ModemDialog : public QDialog
{
    Q_OBJECT

  void connectAll(void);

public:
    explicit ModemDialog(QWidget *parent = 0);
    ~ModemDialog();

    QString path(void) const;
    int baud(void) const;

public slots:
    void onBrowse(void);

private:
    Ui::ModemDialog *ui;
};

#endif // MODEMDIALOG_H

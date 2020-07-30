#ifndef METERUI_H
#define METERUI_H

#include <QWidget>

namespace Ui {
class MeterUI;
}

class MeterUI : public QWidget
{
    Q_OBJECT

public:
    explicit MeterUI(QWidget *parent = 0);
    ~MeterUI();

private:
    Ui::MeterUI *ui;
};

#endif // METERUI_H

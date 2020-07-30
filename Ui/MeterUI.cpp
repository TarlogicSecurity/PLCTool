#include "MeterUI.h"
#include "ui_MeterUI.h"

MeterUI::MeterUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeterUI)
{
    ui->setupUi(this);
}

MeterUI::~MeterUI()
{
    delete ui;
}

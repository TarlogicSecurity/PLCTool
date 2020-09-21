#include "MainWindow.h"
#include "ui_MainWindow.h"


QSaneMdiSubWindow::QSaneMdiSubWindow(QString const &name)
{
  this->name = name;
}


void
QSaneMdiSubWindow::closeEvent(QCloseEvent *)
{
  emit closed(this->name);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->model = new TopologyModel(nullptr, this);
  this->ui->topologyView->setModel(this->model);
  this->ui->topologyView->setSelectionBehavior(QAbstractItemView::SelectRows);
  this->connectAll();
}

void
MainWindow::connectAll(void)
{
  connect(
        this->ui->actionConnect,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toggleStart()));

  connect(
        this->ui->actionSettings,
        SIGNAL(triggered(bool)),
        this,
        SIGNAL(openModemDialog()));

  connect(
        this->ui->actionLoad,
        SIGNAL(triggered(bool)),
        this,
        SIGNAL(loadFile()));

  connect(
        this->ui->topologyView,
        SIGNAL(activated(QModelIndex)),
        this,
        SLOT(onNodeActivated(QModelIndex)));
}

QSaneMdiSubWindow *
MainWindow::openWindow(
    QString const &name,
    QString const &title,
    QWidget *widget)
{
  QSaneMdiSubWindow *subWindow;

  if (findWindow(name) != nullptr)
    return nullptr;

  subWindow = new QSaneMdiSubWindow(name);
  subWindow->setWidget(widget);
  subWindow->setWindowTitle(title);
  this->windowMap[name] = subWindow;

  connect(
        subWindow,
        SIGNAL(closed(QString)),
        this,
        SLOT(onCloseSubWindow(QString)));

  this->ui->mdiArea->addSubWindow(subWindow);

  subWindow->show();

  return subWindow;
}

bool
MainWindow::closeWindow(QString const &name)
{
  QMdiSubWindow *window = this->findWindow(name);

  if (window != nullptr)
    return false;

  return window->close();
}

QSaneMdiSubWindow *
MainWindow::findWindow(QString const &name) const
{
  if (this->windowMap.find(name) != this->windowMap.end())
    return this->windowMap[name];

  return nullptr;
}

void
MainWindow::setSubNet(const PLCTool::SubNet *sn)
{
  this->model->setSubNet(sn);
}

bool
MainWindow::connectState(void) const
{
  return this->ui->actionConnect->isChecked();
}

void
MainWindow::setConnectState(bool state)
{
  this->ui->actionConnect->setChecked(state);
}

void
MainWindow::notifySubNetChanges(void)
{
  bool doResize = this->model->rowCount() < 2;

  this->model->notifyLayout();

  if (doResize) {
    this->ui->topologyView->resizeColumnToContents(0);
    this->ui->topologyView->resizeColumnToContents(1);
  }
}

MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////// Slots ////////////////////////////////////////

void
MainWindow::onCloseSubWindow(QString subWindow)
{
  emit closeSubWindow(subWindow);
  this->windowMap[subWindow] = nullptr;
}

void
MainWindow::onNodeActivated(QModelIndex index)
{
  PLCTool::Node *node = this->model->node(index);

  if (node != nullptr
      && (node->type() == PLCTool::METER
          || node->type() == PLCTool::SWITCH))
    emit openMeterInfo(static_cast<PLCTool::Meter *>(node));
}

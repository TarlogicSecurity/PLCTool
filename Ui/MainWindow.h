#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include "TopologyModel.h"
#include <QHash>

namespace Ui {
class MainWindow;
}

class QSaneMdiSubWindow : public QMdiSubWindow {
  Q_OBJECT

  QString name;

public:
  QSaneMdiSubWindow(QString const &name);

signals:
  void closed(QString);

protected:
  void closeEvent(QCloseEvent *closeEvent);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    TopologyModel *model = nullptr;
    QHash<QString, QSaneMdiSubWindow *> windowMap;

    void connectAll(void);

public:
    explicit MainWindow(QWidget *parent = 0);

    void setSubNet(const PLCTool::SubNet *sn);
    bool connectState(void) const;
    void setConnectState(bool);

    void notifySubNetChanges(void);

    QSaneMdiSubWindow *openWindow(
        QString const &name,
        QString const &title,
        QWidget *widget);
    bool closeWindow(QString const &name);
    QSaneMdiSubWindow *findWindow(QString const &name) const;

    ~MainWindow();

public slots:
    void onCloseSubWindow(QString subWindow);

private:
    Ui::MainWindow *ui;

signals:
    void loadFile(void);
    void toggleStart(void);
    void openModemDialog(void);
    void closeSubWindow(QString);
};

#endif // MAINWINDOW_H

#ifndef UI_UI_H
#define UI_UI_H

#include <QObject>
#include "TopologyModel.h"
#include "Topology/Adapter.h"
#include "ModemDialog.h"
#include "MainWindow.h"


class QtUi : public QObject
{
  Q_OBJECT

  // Ui-related objects
  MainWindow *mainWindow = nullptr;
  ModemDialog *modemDialog = nullptr;

  // UI state
  bool firstConnection = true;

  void connectAll(void);

public:
  explicit QtUi(QObject *parent = 0);
  ~QtUi();

  void show(void);


  // GUI Operations
  void setAdapter(PLCTool::Adapter *);
  void notifyTopologyChange(void);

  void openFrameLog(void);
  void openCredsLog(void);
  void openDlmsLog(void);
  void openTranslator(void);

  void pushFrame(
      const PLCTool::Concentrator *,
      bool downlink,
      const void *data,
      size_t size);

  void pushData(
      const PLCTool::Concentrator *dc,
      PLCTool::NodeId meter,
      bool downlink,
      const void *data,
      size_t size);

  void pushCreds(
      const PLCTool::Concentrator *dc,
      PLCTool::NodeId meter,
      QString password);

  // Convenience getters
  QString modemPath(void) const;
  unsigned int modemBaud(void) const;

signals:
  void openAdapter(void);
  void closeAdapter(void);

public slots:
  void onToggleStart(void);
  void onOpenConfig(void);
};

#endif // UI_UI_H

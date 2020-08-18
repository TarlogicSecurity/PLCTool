#ifndef UI_UI_H
#define UI_UI_H

#include <QObject>
#include "TopologyModel.h"
#include "Topology/Adapter.h"
#include "ModemDialog.h"
#include "MainWindow.h"
#include "LoadingStatusDialog.h"
#include "FrameLogUI.h"
#include "DLMSLogUI.h"
#include "MeterUI.h"
#include <QMap>

#include <QElapsedTimer>

struct Frame;
struct DlmsMessage;
class MeterInfo;

class QtUi : public QObject
{
  Q_OBJECT

  unsigned int parsedFrameCounter = 0;
  unsigned int totalFrameCounter = 0;

  // Ui-related objects
  MainWindow *mainWindow = nullptr;
  ModemDialog *modemDialog = nullptr;

  LoadingStatusDialog *loadingDialog = nullptr;

  FrameLogUI *frameLogUi = nullptr;
  DLMSLogUI *dlmsLogUi = nullptr;
  QElapsedTimer refreshTimer;
  QMap<PLCTool::NodeId, MeterUI *> meterUiMap;

  // UI state
  bool firstConnection = true;

  void breathe(void);
  void connectAll(void);

public:
  explicit QtUi(QObject *parent = 0);
  ~QtUi();

  void show(void);
  void setCounters(unsigned int, unsigned int);
  void setLoading(bool);
  void loadingMessage(QString);

  // GUI Operations
  void setAdapter(PLCTool::Adapter *);
  void notifyTopologyChange(void);

  void openFrameLog(void);
  void openCredsLog(void);
  void openDlmsLog(void);
  void openTranslator(void);

  void openMeterInfoView(MeterInfo *);
  void closeAllMeterInfo(void);

  void pushFrame(Frame const &);
  void pushDlmsMessage(DlmsMessage const &);

  void pushCreds(
      const PLCTool::Concentrator *dc,
      QDateTime,
      PLCTool::NodeId meter,
      QString password,
      QString conformance);

  void refreshViews(void);

  // Convenience getters
  QString modemPath(void) const;
  unsigned int modemBaud(void) const;

signals:
  void openAdapter(void);
  void openMeterInfo(PLCTool::Meter *);
  void openLogFile(QString);
  void closeAdapter(void);

public slots:
  void onLoadFile(void);
  void onToggleStart(void);
  void onOpenConfig(void);
  void onSelectFrame(Frame &);
  void onSelectDlmsMessage(DlmsMessage &);
  void onRejectLoading(void);
};

#endif // UI_UI_H

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>
#include "Topology/Concentrator.h"
#include "Topology/Meter.h"
#include "Ui/QtUi.h"
#include "PRIME/PrimeAdapter.h"
#include "QVector"

#include "PRIMEProcessor.h"
#include "DLMSProcessor.h"

class Application : public QApplication
{
  Q_OBJECT

  // Data model objects
  PLCTool::PrimeAdapter *adapter = nullptr;
  QVector<PLCTool::SubNet *> subNetHistory;

  QThread *primeProcThread = nullptr;
  QThread *dlmsProcThread = nullptr;

  PRIMEProcessor *primeProcessor = nullptr;
  DLMSProcessor *dlmsProcessor = nullptr;

  // UI objects
  QtUi *ui = nullptr;

  void parseDataFrame(
      PLCTool::Meter *meter,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);

  void connectAdapter(void);
  void connectUi(void);
  void connectProcessors(void);

public:
  explicit Application(int &argc, char *argv[]);
  ~Application();

  bool openAdapter(QString const &path, unsigned int baud = 921600);
  bool loadLogFile(QString const &path);
  bool closeAdapter(void);

  bool work(void);

signals:
  void frameReceived(
      quint64 dcId,
      QDateTime timeStamp,
      bool downlink,
      QVector<uint8_t>);

  void messageReceived(
      QString SNA,
      QDateTime timeStamp,
      quint64 id,
      bool downlink,
      QVector<uint8_t>);

public slots:
  void onSubnetAnnounce(
      PLCTool::Concentrator *,
      QDateTime,
      uint64_t times);
  void onMeterFound(
      PLCTool::Concentrator *,
      QDateTime,
      PLCTool::Meter *);
  void onFrameReceived(
      PLCTool::Concentrator *,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);
  void onDataReceived(
      PLCTool::Meter *meter,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);

  void onAdapterStatusMessage(QString);
  void onAdapterClosed(void);
  void onAdapterRefreshRequested(void);
  void onOpenAdapter(void);
  void onOpenLogFile(QString);
  void onCloseAdapter(void);

  void onProcessedFrame(Frame);
  void onProcessedDlmsMessage(DlmsMessage);
};

#endif // APPLICATION_H

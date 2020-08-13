#ifndef DLMSPROCESSOR_H
#define DLMSPROCESSOR_H

#include <QObject>
#include <PRIME/PrimeAdapter.h>
#include <GXDLMSTranslator.h>
#include <QVector>

struct DlmsMessage {
  QDateTime timeStamp;
  QString SNA;
  PLCTool::NodeId nodeId;
  QString type;
  bool downlink;
  QVector<uint8_t> pdu;
  QString xml;

  void setDlmsData(const uint8_t *data, size_t size);
  QString toText(void) const;
};


class DLMSProcessor : public QObject
{
  Q_OBJECT

  CGXDLMSTranslator translator;

public:
  explicit DLMSProcessor(QObject *parent = 0);

  static void registerTypes(void);

signals:
  void dlmsMessage(DlmsMessage);

public slots:
  void process(
      QString SNA,
      QDateTime timeStamp,
      quint64 id,
      bool downlink,
      QVector<uint8_t> data);
};

#endif // DLMSPROCESSOR_H

#ifndef METERCONTEXT_H
#define METERCONTEXT_H

#include <QObject>
#include <Topology/Meter.h>
#include "PRIMEProcessor.h"
#include "DLMSProcessor.h"
#include <QVector>

struct CredInfo {
  QDateTime timeStamp;
  QString password;
  QString context;
};

class MeterInfo : public QObject
{
  Q_OBJECT

  PLCTool::Meter *mMeter;

  QDateTime lastActivity;

  QString mMac = "N/A";
  QVector<Frame> frames;
  QVector<DlmsMessage> messages;
  QVector<CredInfo> creds;

public:
  explicit MeterInfo(QObject *parent, PLCTool::Meter *meter);

  void pushFrame(Frame const &);
  void pushDlmsMessage(DlmsMessage const &);
  void pushCreds(QDateTime timeStamp, QString password, QString ctx);

  inline QVector<CredInfo> *
  credList(void)
  {
    return &this->creds;
  }

  inline QVector<Frame> *
  frameList(void)
  {
    return &this->frames;
  }

  inline QVector<DlmsMessage> *
  messageList(void)
  {
    return &this->messages;
  }

  inline PLCTool::Meter *
  meter(void) const
  {
    return this->mMeter;
  }

  QString
  mac(void) const
  {
    return this->mMac;
  }

signals:
  void messageReceived(DlmsMessage);
  void frameReceived(Frame);
  void credentialsFound(QDateTime, QString, QString);

public slots:
};

#endif // METERCONTEXT_H

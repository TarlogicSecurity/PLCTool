#ifndef METERCONTEXT_H
#define METERCONTEXT_H

#include <QObject>
#include <Topology/Meter.h>
#include "PRIMEProcessor.h"
#include "DLMSProcessor.h"
#include <QVector>

class MeterInfo;

struct CredInfo  {
  MeterInfo *owner;
  QDateTime timeStamp;
  QString password;
  QStringList contexts;

  CredInfo(CredInfo const &);
  CredInfo();

  static void registerTypes(void);
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

  QVector<Frame> pendingFrames;
  QVector<DlmsMessage> pendingMessages;
  QVector<CredInfo> pendingCreds;

public:
  explicit MeterInfo(QObject *parent, PLCTool::Meter *meter);

  void pushFrame(Frame const &);
  void pushDlmsMessage(DlmsMessage const &);
  CredInfo pushCreds(
      QDateTime const &timeStamp,
      QString const &password,
      QStringList const &ctx);

  inline QVector<CredInfo> *
  pendingCredList(void)
  {
    return &this->pendingCreds;
  }

  inline QVector<CredInfo> *
  credList(void)
  {
    return &this->creds;
  }

  inline void
  commitCreds(void)
  {
    this->pendingCreds.clear();
  }

  inline QVector<Frame> *
  pendingFrameList(void)
  {
    return &this->pendingFrames;
  }

  inline QVector<Frame> *
  frameList(void)
  {
    return &this->frames;
  }

  inline void
  commitFrames(void)
  {
    this->pendingFrames.clear();
  }

  inline QVector<DlmsMessage> *
  pendingMessageList(void)
  {
    return &this->pendingMessages;
  }

  inline QVector<DlmsMessage> *
  messageList(void)
  {
    return &this->messages;
  }

  inline void
  commitMessages(void)
  {
    this->pendingMessages.clear();
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
  void credentialsFound(CredInfo);

public slots:
};

#endif // METERCONTEXT_H

#ifndef PRIMEPROCESSOR_H
#define PRIMEPROCESSOR_H

#include <QObject>
#include <PRIME/PrimeFrame.h>
#include <QDateTime>
#include <QVector>

struct Frame {
  QString SNA;
  bool downlink;
  QVector<uint8_t> bytes;
  QDateTime timeStamp;
  PLCTool::PrimeFrame *frame = nullptr;

  Frame();
  Frame(const Frame &);
  QString toHtml(void) const;

  ~Frame();
};

class PRIMEProcessor : public QObject
{
  Q_OBJECT

public:
  explicit PRIMEProcessor(QObject *parent = 0);

  static void registerTypes(void);

signals:
  void frame(Frame);

public slots:
  void process(
      quint64 dcId,
      QDateTime timeStamp,
      bool downlink,
      QVector<uint8_t>);
};

#endif // PRIMEPROCESSOR_H

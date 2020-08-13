#include "DLMSProcessor.h"
#include <dlms/dlmsmsg.h>

Q_DECLARE_METATYPE(DlmsMessage)
Q_DECLARE_METATYPE(DlmsMessage *)
static bool typeRegistered = false;


QString
DlmsMessage::toText(void) const
{
  QString asHex;
  char string[4];

  if (this->xml.size() > 0)
    return this->xml;

  for (int i = 0; i < this->pdu.size(); ++i) {
    snprintf(string, sizeof(string), "%02x", this->pdu[i]);
    asHex += string;
  }

  return asHex;
}

///////////////////////////////////////////////////////////////////////////////

DLMSProcessor::DLMSProcessor(QObject *parent) :
  QObject(parent), translator(DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
{
  this->translator.SetComments(true);
}

void
DLMSProcessor::registerTypes(void)
{
  if (!typeRegistered) {
    qRegisterMetaType<DlmsMessage>();
    qRegisterMetaType<DlmsMessage *>();
    typeRegistered = true;
  }
}

void
DLMSProcessor::process(
    QString SNA,
    QDateTime timeStamp,
    quint64 id,
    bool downlink,
    QVector<uint8_t> data)
{
  DlmsMessage msg;
  CGXByteBuffer dataBuffer;
  std::string str;

  msg.downlink = downlink;
  msg.SNA = SNA;
  msg.nodeId = id;
  msg.timeStamp = timeStamp;

  msg.pdu = data;;

  if (msg.pdu.size() > 0)
    msg.type = dlms_command_to_str(
          static_cast<DLMS_COMMAND>(msg.pdu[0]));
  else
    msg.type = "<!-- empty -->";


  dataBuffer.Set(msg.pdu.data(), msg.pdu.size());
  if (this->translator.PduToXml(dataBuffer, str) == 0) {
    msg.xml = QString::fromStdString(str);
    emit dlmsMessage(msg);
  }
}

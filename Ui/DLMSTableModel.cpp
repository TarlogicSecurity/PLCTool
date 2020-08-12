#include "DLMSTableModel.h"
#include <QColor>

DLMSTableModel::DLMSTableModel(QObject *parent, QVector<DlmsMessage> *list)
  : QAbstractTableModel(parent), messageList(list)
{
}

int
DLMSTableModel::rowCount(const QModelIndex &) const
{
  if (this->messageList == nullptr)
    return 0;

  return this->messageList->size();
}

int
DLMSTableModel::columnCount(const QModelIndex &) const
{
  return 8;
}

QVariant
DLMSTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole
      && orientation == Qt::Horizontal
      && section >= 0 && section < 8) {
    const char *sections[] = {
      "ID", "Timestamp", "Direction", "Type",
      "LNID", "SNA", "Size", "PDU"
    };

    return QString(sections[section]);
  }

  return QVariant();
}

QVariant
DLMSTableModel::data(const QModelIndex &index, int role) const
{
  int column = index.column();
  DlmsMessage *entry = nullptr;

  if (this->messageList == nullptr)
    return QVariant();

  if (index.row() < 0 || index.row() >= this->messageList->size())
    return QVariant();

  entry = &(*this->messageList)[index.row()];

  if (role == Qt::DisplayRole) {
    char lnid[8] = "ffffff";
    QString byteString;

    switch (column) {
      case 0:
        return QVariant::fromValue(index.row() + 1);

      case 1:
        return entry->timeStamp.toString();

      case 2:
        return entry->downlink ? QString("D") : QString("U");

      case 3:
        return entry->type;

      case 4:
        snprintf(
              lnid,
              sizeof(lnid),
              "%06lx",
              entry->nodeId);

        return QString(lnid);

      case 5:
        return entry->SNA;

      case 6:
        return QVariant::fromValue(entry->pdu.size());

      case 7:
        for (int i = 0; i < entry->pdu.size(); ++i) {
          char byte[4];
          snprintf(byte, 4, "%02x", entry->pdu[i]);
          byteString += QString(byte) + " ";
        }

        return byteString;
    }

    return QVariant();
  } else if (role == Qt::UserRole) {
    return QVariant::fromValue(index.row());
  } else if (role == Qt::BackgroundColorRole) {
    QColor bg = QColor::fromRgb(0xff, 0xff, 0xff);

    if (entry->type == "AARQ")
      bg = QColor::fromRgb(0x40, 0x80, 0x40);
    else if (entry->type == "AARE")
      bg = QColor::fromRgb(0x80, 0xff, 0xff);
    else if (entry->type == "METHOD_REQUEST")
      bg = QColor::fromRgb(0x80, 0x40, 0x40);
    else if (entry->type == "GET_REQUEST")
      bg = QColor::fromRgb(0x80, 0xff, 0x80);
    else if (entry->type == "SET_REQUEST")
      bg = QColor::fromRgb(0xff, 0xff, 0x80);
    else if (entry->type == "EVENT_NOTIFICATION")
      bg = QColor::fromRgb(0x80, 0x40, 0x40);
    else if (entry->type.startsWith("DED_"))
      bg = QColor::fromRgb(0x00, 0x00, 0x40);
    else if (entry->type.startsWith("GLO_"))
      bg = QColor::fromRgb(0x00, 0x00, 0x00);
    return QVariant::fromValue(bg);
  } else if (role == Qt::ForegroundRole) {
    if (entry->type == "AARQ"
        || entry->type == "METHOD_REQUEST"
        || entry->type == "EVENT_NOTIFICATION"
        || entry->type.startsWith("DED_")
        || entry->type.startsWith("GLO_"))
      return QVariant::fromValue(QColor::fromRgb(0xff, 0xff, 0xff));
    else
      return QVariant::fromValue(QColor::fromRgb(0, 0, 0));
  }

  return QVariant();
}

void
DLMSTableModel::refreshData(void)
{
  if (this->messageList->size() > this->lastSize) {
    this->beginInsertRows(
          QModelIndex(),
          this->lastSize,
          this->messageList->size() - 1);
    this->endInsertRows();
    emit dataChanged(
          createIndex(this->lastSize, 0),
          createIndex(this->messageList->size() - 1, 7));
  } else if (this->messageList->size() == 0
             && this->messageList->size() != this->lastSize) {
    this->beginRemoveRows(
          QModelIndex(),
          0,
          this->lastSize - 1);
    this->endRemoveRows();
    emit dataChanged(
          createIndex(0, 0),
          createIndex(0, 0));
  }

  this->lastSize = this->messageList->size();
}

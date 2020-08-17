#include "FrameTableModel.h"
#include <QColor>

void
FrameTableModel::registerTypes(void)
{
  PRIMEProcessor::registerTypes();
}


FrameTableModel::FrameTableModel(QObject *parent, QVector<Frame> *list)
  : QAbstractTableModel(parent), frameList(list)
{
  this->registerTypes();
}

int
FrameTableModel::rowCount(const QModelIndex &) const
{
  if (this->frameList == nullptr)
    return 0;

  return this->frameList->size();
}

int
FrameTableModel::columnCount(const QModelIndex &) const
{
  return 8;
}

QVariant
FrameTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole
      && orientation == Qt::Horizontal
      && section >= 0 && section < 8) {
    const char *sections[] = {
      "ID", "Timestamp", "Direction", "Type",
      "LNID", "SNA", "Size", "Bytes"
    };

    return QString(sections[section]);
  }

  return QVariant();
}

QVariant
FrameTableModel::data(const QModelIndex &index, int role) const
{
  int column = index.column();
  Frame *entry = nullptr;

  if (this->frameList == nullptr)
    return QVariant();

  if (index.row() < 0 || index.row() >= this->frameList->size())
    return QVariant();

  entry = &(*this->frameList)[index.row()];

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
        return entry->frame == nullptr
            ? QString("Corrupt")
            : QString::fromStdString(entry->frame->typeToString());

      case 4:
        if (entry->frame != nullptr) {
          snprintf(
                lnid,
                sizeof(lnid),
                "%06x",
                entry->frame->PDU.macType == PLCTool::PrimeFrame::GENERIC
                 ? entry->frame->PDU.PKT.LNID
                 : 0xffffff);
        }

        return QString(lnid);

      case 5:
        return entry->SNA;

      case 6:
        return QVariant::fromValue(entry->bytes.size());

      case 7:
        for (int i = 0; i < entry->bytes.size(); ++i) {
          char byte[4];
          snprintf(byte, 4, "%02x", entry->bytes[i]);
          byteString += QString(byte) + " ";
        }

        return byteString;
    }

    return QVariant();
  } else if (role == Qt::UserRole) {
    return QVariant::fromValue(index.row());
  } else if (role == Qt::BackgroundColorRole) {
    QColor bg = QColor::fromRgb(0xff, 0xff, 0xff);

    if (entry->frame == nullptr) {
      bg = QColor::fromRgb(0xff, 0x80, 0x80);
    } else if (entry->frame->PDU.macType == PLCTool::PrimeFrame::BEACON) {
      bg = QColor::fromRgb(0xc0, 0xc0, 0xc0);
    } else if (entry->frame->PDU.macType == PLCTool::PrimeFrame::GENERIC) {
      if (entry->frame->PDU.genType == PLCTool::PrimeFrame::DATA)
        bg = QColor::fromRgb(0x80, 0xff, 0x80);
    }

    return QVariant::fromValue(bg);
  } else if (role == Qt::ForegroundRole) {
    return QVariant::fromValue(QColor::fromRgb(0, 0, 0));
  }

  return QVariant();
}

void
FrameTableModel::refreshData(void)
{
  if (this->frameList->size() > this->lastSize) {
    emit dataChanged(
          createIndex(this->lastSize, 0),
          createIndex(this->frameList->size() - 1, 7));
  } else if (this->frameList->size() == 0
             && this->frameList->size() != this->lastSize) {
    emit layoutChanged();
  }

  this->lastSize = this->frameList->size();
}


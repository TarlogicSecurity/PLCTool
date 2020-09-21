#include "CredInfoTableModel.h"
#include <PRIME/PrimeAdapter.h>
#include <QAction>

CredInfoTableModel::CredInfoTableModel(QObject *parent, QVector<CredInfo> *list)
  : QAbstractTableModel(parent), credInfoList(list)
{
}

int
CredInfoTableModel::rowCount(const QModelIndex &) const
{
  if (this->credInfoList == nullptr)
    return 0;

  return this->credInfoList->size();
}

int
CredInfoTableModel::columnCount(const QModelIndex &) const
{
  return 8;
}

QVariant
CredInfoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole
      && orientation == Qt::Horizontal
      && section >= 0 && section < 8) {
    const char *sections[] = {
      "Id", "Timestamp", "Meter name", "NID",
      "MAC", "SNA", "Password", "Contexts"
    };

    return QString(sections[section]);
  }

  return QVariant();
}

QVariant
CredInfoTableModel::data(const QModelIndex &index, int role) const
{
  int column = index.column();
  CredInfo *entry = nullptr;

  if (!index.isValid())
    return QVariant();

  if (this->credInfoList == nullptr)
    return QVariant();

  if (index.row() < 0 || index.row() >= this->credInfoList->size())
    return QVariant();

  entry = &(*this->credInfoList)[index.row()];

  if (role == Qt::DisplayRole) {
    char lnid[8] = "ffffff";
    QString byteString;

    switch (column) {
      case 0:
        return index.row() + 1;

      case 1:
        return entry->timeStamp.toString();

      case 2:
        if (entry->owner->meter()->name().size() > 0)
            return QString::fromStdString(entry->owner->meter()->name());

        return QString("N/A");

      case 3:
        snprintf(
              lnid,
              sizeof(lnid),
              "%06lx",
              entry->owner->meter()->id());

        return QString(lnid);

      case 4:
        if (entry->owner->meter()->macAddr().size() > 0)
            return QString::fromStdString(entry->owner->meter()->macAddr());

        return "N/A";

      case 5:
        return
            QString::fromStdString(
              PLCTool::PrimeAdapter::idToSna(
                entry->owner->meter()->parent()->parent()->id()));

      case 6:
        return entry->password;

      case 7:
        return entry->contexts.join(", ");
    }

    return QVariant();
  } else if (role == Qt::UserRole) {
    return QVariant::fromValue(index.row());
  } else if (role == Qt::BackgroundColorRole) {
    if (entry->contexts.contains("Action")
        || entry->contexts.contains("BlockTransferWithAction")) {
      return QColor::fromRgb(0xff, 0x7f, 0x7f);
    } else if (entry->contexts.contains("BlockTransferWithSetOrWrite")
        || entry->contexts.contains("Write")
        || entry->contexts.contains("Attribute0SupportedWithSet")
        || entry->contexts.contains("Unconfirmed-write")) {
      return  QColor::fromRgb(0xff, 0xff, 0x7f);
    }

    return QColor::fromRgb(0xff, 0xff, 0xff);
  } else if (role == Qt::ForegroundRole) {
    return QVariant::fromValue(QColor::fromRgb(0, 0, 0));
  }

  return QVariant();
}

void
CredInfoTableModel::appendData(QVector<CredInfo> const &newData)
{
  if (newData.size() > 0) {
    beginInsertRows(
          QModelIndex(),
          this->credInfoList->size(),
          this->credInfoList->size() + newData.size() - 1);

    this->credInfoList->append(newData);

    endInsertRows();
  }
}

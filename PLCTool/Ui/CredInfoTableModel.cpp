//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "CredInfoTableModel.h"

#include <PLCTool/PRIME/PrimeAdapter.h>

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
CredInfoTableModel::headerData(
    int section,
    Qt::Orientation orientation,
    int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal &&
      section >= 0 && section < 8) {
    const char *sections[] = {
        "Id",
        "Timestamp",
        "Meter name",
        "NID",
        "MAC",
        "SNA",
        "Password",
        "Contexts"};

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
        snprintf(lnid, sizeof(lnid), "%06lx", entry->owner->meter()->id());

        return QString(lnid);

      case 4:
        if (entry->owner->meter()->macAddr().size() > 0)
          return QString::fromStdString(entry->owner->meter()->macAddr());

        return "N/A";

      case 5:
        return QString::fromStdString(PLCTool::PrimeAdapter::idToSna(
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
    if (entry->contexts.contains("Action") ||
        entry->contexts.contains("BlockTransferWithAction")) {
      return QColor::fromRgb(0xff, 0x7f, 0x7f);
    } else if (
        entry->contexts.contains("BlockTransferWithSetOrWrite") ||
        entry->contexts.contains("Set") || entry->contexts.contains("Write") ||
        entry->contexts.contains("Attribute0SupportedWithSet") ||
        entry->contexts.contains("Unconfirmed-write")) {
      return QColor::fromRgb(0xff, 0xff, 0x7f);
    }

    return QColor::fromRgb(0xff, 0xff, 0xff);
  } else if (role == Qt::ForegroundRole) {
    return QVariant::fromValue(QColor::fromRgb(0, 0, 0));
  }

  return QVariant();
}

void
CredInfoTableModel::clearData(void)
{
  if (this->credInfoList->size() > 0) {
    beginRemoveRows(QModelIndex(), 0, this->credInfoList->size() - 1);
    this->credInfoList->clear();
    endRemoveRows();
  }
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

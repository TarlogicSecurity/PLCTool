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

  if (!index.isValid())
    return QVariant();

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
DLMSTableModel::clearData(void)
{
  if (this->messageList->size() > 0) {
    beginRemoveRows(QModelIndex(), 0, this->messageList->size() - 1);
    this->messageList->clear();
    endRemoveRows();
  }
}

void
DLMSTableModel::appendData(QVector<DlmsMessage> const &newData)
{
  if (newData.size() > 0) {
    beginInsertRows(
          QModelIndex(),
          this->messageList->size(),
          this->messageList->size() + newData.size() - 1);

    this->messageList->append(newData);

    endInsertRows();
  }
}

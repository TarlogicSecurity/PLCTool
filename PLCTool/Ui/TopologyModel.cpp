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

#include "TopologyModel.h"

#include <PLCTool/PRIME/PrimeAdapter.h>
#include <PLCTool/Topology/Concentrator.h>
#include <PLCTool/Topology/Meter.h>
#include <PLCTool/Topology/Switch.h>

#include <QColor>

Q_DECLARE_METATYPE(PLCTool::Hub);
Q_DECLARE_METATYPE(PLCTool::Node);
Q_DECLARE_METATYPE(PLCTool::Node *);
Q_DECLARE_METATYPE(PLCTool::Concentrator);
Q_DECLARE_METATYPE(PLCTool::Switch);
Q_DECLARE_METATYPE(PLCTool::Meter);

TopologyModel::TopologyModel(const PLCTool::SubNet *net, QObject *parent)
    : QAbstractItemModel(parent)
{
  this->setSubNet(net);
}

void
TopologyModel::setSubNet(const PLCTool::SubNet *net)
{
  this->net = net;
  emit layoutChanged();
}

void
TopologyModel::notifyData(void)
{
  // TODO: Implement me!
}

void
TopologyModel::notifyLayout(void)
{
  emit layoutChanged();
}

QVariant
TopologyModel::headerData(int section, Qt::Orientation orientation, int role)
    const
{
  if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return QString("Node");
        break;

      case 1:
        return QString("Name");
        break;

      case 2:
        return QString("MAC");
        break;
    }
  }

  return QVariant();
}

QModelIndex
TopologyModel::index(int row, int column, const QModelIndex &parent) const
{
  QModelIndex index;

  const PLCTool::SubNet *net = nullptr;

  if (!hasIndex(row, column, parent)) {
    return QModelIndex();
  }
  // Root element. Querying entry in subnet

  net = this->net;

  if (!parent.isValid()) {
    net = this->net;
  } else {
    // Populated node.
    PLCTool::Node *current =
        static_cast<PLCTool::Node *>(parent.internalPointer());

    if (current != nullptr && current->isHub()) {
      PLCTool::Hub *asHub = static_cast<PLCTool::Hub *>(current);
      net = &asHub->subNet();
    }
  }

  if (net == nullptr || row < 0 ||
      static_cast<unsigned>(row) >= net->listCount()) {
    return QModelIndex();
  }
  index = createIndex(row, column, net->listNodeAt(row));

  return index;
}

QModelIndex
TopologyModel::parent(const QModelIndex &index) const
{
  QModelIndex newIndex;

  if (!index.isValid()) {
    return QModelIndex();
  }

  PLCTool::Node *current =
      static_cast<PLCTool::Node *>(index.internalPointer());

  if (current == nullptr) {
    return QModelIndex();
  }

  PLCTool::SubNet *sn = current->parent();

  if (sn == nullptr || sn->parent() == nullptr) {
    return QModelIndex();
  }

  newIndex = createIndex(sn->parent()->listNdx(), 0, sn->parent());

  return newIndex;
}

int
TopologyModel::rowCount(const QModelIndex &parent) const
{
  PLCTool::Node *current =
      static_cast<PLCTool::Node *>(parent.internalPointer());

  if (!parent.isValid())  // Root?
    return this->net == nullptr ? 0 : this->net->listCount();

  if (current->isHub()) {
    PLCTool::Hub *asHub = static_cast<PLCTool::Hub *>(current);

    return asHub->subNet().listCount();
  }

  return 0;
}

int
TopologyModel::columnCount(const QModelIndex &) const
{
  return 3;
}

bool
TopologyModel::hasChildren(const QModelIndex &parent) const
{
  return this->rowCount(parent) > 0;
}

bool
TopologyModel::canFetchMore(const QModelIndex &) const
{
  return false;
}

void
TopologyModel::fetchMore(const QModelIndex &)
{
  // FIXME: Implement me!
}

PLCTool::Node *
TopologyModel::node(QModelIndex const &index) const
{
  if (!index.isValid())
    return nullptr;

  return static_cast<PLCTool::Node *>(index.internalPointer());
}

QVariant
TopologyModel::data(const QModelIndex &index, int role) const
{
  PLCTool::Node *current = this->node(index);

  switch (role) {
    case Qt::DisplayRole:
      switch (current->type()) {
        PLCTool::Meter *asMeter;
        PLCTool::Concentrator *asDc;

        case PLCTool::NodeType::UNDEFINED:
          return QVariant::fromValue(QString("UNDEFINED"));
          break;

        case PLCTool::NodeType::HUB:
          asMeter = static_cast<PLCTool::Meter *>(current);
          if (index.column() == 0) {
            return QVariant::fromValue(
                QString::asprintf("SWITCH %02x", asMeter->subNet().netId()));
          } else if (index.column() == 1) {
            if (asMeter->name().size() > 0)
              return QString::fromStdString(asMeter->name());
            else
              return "(unseen switch)";
          } else if (index.column() == 2) {
            if (asMeter->macAddr().size() > 0)
              return QString::fromStdString(asMeter->macAddr());
            else
              return "(not seen)";
          }
          break;

        case PLCTool::NodeType::METER:
        case PLCTool::NodeType::SWITCH:
          asMeter = static_cast<PLCTool::Meter *>(current);
          if (index.column() == 0) {
            return current->type() == PLCTool::NodeType::SWITCH
                       ? QVariant::fromValue(QString::asprintf(
                             "SWITCH %02x (%06lx)",
                             asMeter->subNet().netId(),
                             asMeter->id()))
                       : QVariant::fromValue(
                             QString::asprintf("NID: %06lx", asMeter->id()));
          } else if (index.column() == 1) {
            if (asMeter->name().size() > 0)
              return QString::fromStdString(asMeter->name());
            else
              return current->type() == PLCTool::NodeType::SWITCH
                         ? "(unnamed switch)"
                         : "(unnamed meter)";
          } else if (index.column() == 2) {
            if (asMeter->macAddr().size() > 0)
              return QString::fromStdString(asMeter->macAddr());
            else
              return "(not seen)";
          }
          break;

        case PLCTool::NodeType::CONCENTRATOR:
          asDc = static_cast<PLCTool::Concentrator *>(current);
          if (index.column() == 0) {
            return QVariant::fromValue(
                QString::asprintf("SNA: %06lx", asDc->id()));
          } else if (index.column() == 1) {
            return "(unnamed DC)";
          } else if (index.column() == 2) {
            return QVariant::fromValue(QString::fromStdString(
                PLCTool::PrimeAdapter::idToSna(asDc->id())));
          }
          break;
      }
      break;

    case Qt::ForegroundRole:
    case Qt::BackgroundRole:
      QColor fg = QColor::fromRgb(0, 0, 0);
      QColor bg = QColor::fromRgb(0xff, 0xff, 0xff);

      if (current->type() == PLCTool::NodeType::METER) {
        PLCTool::Meter *asMeter = static_cast<PLCTool::Meter *>(current);
        if (asMeter->params().contains("AARQ_FOUND"))
          bg = QColor::fromRgb(0xff, 0xff, 0);
      }

      return role == Qt::ForegroundRole ? fg : bg;
  }

  return QVariant();
}

bool
TopologyModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row + count - 1);
  // FIXME: Implement me!
  endInsertRows();

  return false;
}

bool
TopologyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
  beginInsertColumns(parent, column, column + count - 1);
  // FIXME: Implement me!
  endInsertColumns();

  return false;
}

bool
TopologyModel::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, row + count - 1);
  // FIXME: Implement me!
  endRemoveRows();

  return false;
}

bool
TopologyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
  beginRemoveColumns(parent, column, column + count - 1);
  // FIXME: Implement me!
  endRemoveColumns();

  return false;
}

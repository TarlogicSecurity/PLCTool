#include "TopologyModel.h"
#include <Topology/Meter.h>
#include <Topology/Switch.h>
#include <Topology/Concentrator.h>

#include <PRIME/PrimeAdapter.h>

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
TopologyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole) {
    if (section == 0)
      return QVariant(QString("Node"));
    else
      return QVariant(QString("Description"));
  }

  return QVariant();
}

QModelIndex
TopologyModel::index(int row, int column, const QModelIndex &parent) const
{
  const PLCTool::SubNet *net = nullptr;

  // Root element. Querying entry in subnet
  if (!parent.isValid()) {
    net = this->net;
  } else {
    // Populated node.
    PLCTool::Node *current = static_cast<PLCTool::Node *>(parent.internalPointer());

    if (current != nullptr && current->isHub()) {
      PLCTool::Hub *asHub = static_cast<PLCTool::Hub *>(current);
      net = &asHub->subNet();
    }
  }

  if (net == nullptr
        || row < 0
        || static_cast<unsigned>(row) >= net->length())
    return QModelIndex();

  return createIndex(row, column, net->nodeAt(row));
}

QModelIndex
TopologyModel::parent(const QModelIndex &index) const
{
  PLCTool::Node *current = static_cast<PLCTool::Node *>(index.internalPointer());


  if (current == nullptr)
    return QModelIndex();

  PLCTool::SubNet *sn = current->parent();

  if (sn == nullptr)
    return QModelIndex();

  if (sn->parent() == nullptr)
    return QModelIndex();

  return createIndex(sn->parent()->allocNdx(), index.column(), sn->parent());
}

int
TopologyModel::rowCount(const QModelIndex &parent) const
{
  PLCTool::Node *current = static_cast<PLCTool::Node *>(parent.internalPointer());


  if (!parent.isValid())   // Root?
    return this->net == nullptr ? 0 : this->net->length();

  if (current->type() == PLCTool::NodeType::SWITCH
      || current->type() == PLCTool::NodeType::CONCENTRATOR) {
    PLCTool::Hub *asHub = static_cast<PLCTool::Hub *>(current);

    return asHub->subNet().length();
  }

  return 0;
}

int
TopologyModel::columnCount(const QModelIndex &) const
{
  return 2;
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

QVariant
TopologyModel::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (!index.isValid()) {
    return QVariant();
  } else {
    PLCTool::Node *current = static_cast<PLCTool::Node *>(index.internalPointer());

    switch (current->type()) {
      PLCTool::Meter *asMeter;
      PLCTool::Switch *asSwitch;
      PLCTool::Concentrator *asDc;

      case PLCTool::NodeType::UNDEFINED:
        return QVariant::fromValue(QString("UNDEFINED"));
        break;

      case PLCTool::NodeType::METER:
        asMeter = static_cast<PLCTool::Meter *>(current);
        if (index.column() == 0)
          return QVariant::fromValue(
                QString().sprintf("LNID: %06lx", asMeter->id()));
        else if (index.column() == 1)
          return QVariant::fromValue(QString("PRIME Meter"));
        break;

      case PLCTool::NodeType::SWITCH:
        asSwitch = static_cast<PLCTool::Switch *>(current);
        if (index.column() == 0)
          return QVariant::fromValue(
                QString().sprintf("LNID: %06lx", asSwitch->id()));
        else if (index.column() == 1)
          return QVariant::fromValue(QString("PRIME Switch"));
        break;

      case PLCTool::NodeType::CONCENTRATOR:
        asDc = static_cast<PLCTool::Concentrator *>(current);
        if (index.column() == 0)
          return QVariant::fromValue(QString().sprintf("SNA: %06lx", asDc->id()));
        else if (index.column() == 1)
          return QVariant::fromValue(
                QString::fromStdString(
                  PLCTool::PrimeAdapter::idToSna(
                    asDc->id())));
        break;
    }
  }

  return QVariant();
}

bool TopologyModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row + count - 1);
  // FIXME: Implement me!
  endInsertRows();

  return false;
}

bool TopologyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
  beginInsertColumns(parent, column, column + count - 1);
  // FIXME: Implement me!
  endInsertColumns();

  return false;
}

bool TopologyModel::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, row + count - 1);
  // FIXME: Implement me!
  endRemoveRows();

  return false;
}

bool TopologyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
  beginRemoveColumns(parent, column, column + count - 1);
  // FIXME: Implement me!
  endRemoveColumns();

  return false;
}

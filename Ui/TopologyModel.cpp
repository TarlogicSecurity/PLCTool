#include "TopologyModel.h"
#include <Topology/Meter.h>
#include <Topology/Switch.h>
#include <Topology/Concentrator.h>
#include <QColor>

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
    switch (section) {
      case 0:
        return QString("Node");
        break;

      case 1:
        return QString("Name");
        break;

      case 2:
        return QString("Credentials");
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
        || static_cast<unsigned>(row) >= net->length()) {
    return QModelIndex();
  }
  index =  createIndex(row, column, net->nodeAt(row));

  return index;
}

QModelIndex
TopologyModel::parent(const QModelIndex &index) const
{
  QModelIndex newIndex;

  if (!index.isValid()) {
    return QModelIndex();
  }

  PLCTool::Node *current = static_cast<PLCTool::Node *>(index.internalPointer());

  if (current == nullptr) {
    return QModelIndex();
  }

  PLCTool::SubNet *sn = current->parent();

  if (sn == nullptr) {
    return QModelIndex();
  }
  if (sn->parent() == nullptr) {
    return QModelIndex();
  }

  newIndex = createIndex(sn->parent()->allocNdx(), 0, sn->parent());

  return newIndex;
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
  if (!index.isValid())
    return QVariant();

  PLCTool::Node *current = static_cast<PLCTool::Node *>(index.internalPointer());

  switch (role) {
    case Qt::DisplayRole:
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
          else if (index.column() == 1) {
            if (asMeter->name().size() > 0)
              return QString::fromStdString(asMeter->name());
            else
              return QVariant::fromValue(QString("(unnamed meter)"));
          }
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

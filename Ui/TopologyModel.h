#ifndef TOPOLOGYMODEL_H
#define TOPOLOGYMODEL_H

#include <QAbstractItemModel>
#include <Topology/SubNet.h>
#include <Topology/Hub.h>

class TopologyModel : public QAbstractItemModel
{
  Q_OBJECT

  const PLCTool::SubNet *net = nullptr;

public:
  explicit TopologyModel(const PLCTool::SubNet *net, QObject *parent = 0);

  void setSubNet(const PLCTool::SubNet *net);

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  // Fetch data dynamically:
  bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

  bool canFetchMore(const QModelIndex &parent) const override;
  void fetchMore(const QModelIndex &parent) override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  // Add data:
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

  // Remove data:
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

  // Retrieve node from index
  PLCTool::Node *node(QModelIndex const &) const;

  // Notifiers
  void notifyData(void);
  void notifyLayout(void);

private:
};

#endif // TOPOLOGYMODEL_H

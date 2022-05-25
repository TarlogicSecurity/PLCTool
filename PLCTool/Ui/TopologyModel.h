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

#ifndef TOPOLOGYMODEL_H
#define TOPOLOGYMODEL_H

#include <PLCTool/Topology/Hub.h>
#include <PLCTool/Topology/SubNet.h>

#include <QAbstractItemModel>

class TopologyModel : public QAbstractItemModel
{
  Q_OBJECT

  const PLCTool::SubNet *net = nullptr;

 public:
  explicit TopologyModel(const PLCTool::SubNet *net, QObject *parent = 0);

  void setSubNet(const PLCTool::SubNet *net);

  // Header:
  QVariant headerData(
      int section,
      Qt::Orientation orientation,
      int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(
      int row,
      int column,
      const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  // Fetch data dynamically:
  bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

  bool canFetchMore(const QModelIndex &parent) const override;
  void fetchMore(const QModelIndex &parent) override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)
      const override;

  // Add data:
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex())
      override;
  bool insertColumns(
      int column,
      int count,
      const QModelIndex &parent = QModelIndex()) override;

  // Remove data:
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex())
      override;
  bool removeColumns(
      int column,
      int count,
      const QModelIndex &parent = QModelIndex()) override;

  // Retrieve node from index
  PLCTool::Node *node(QModelIndex const &) const;

  // Notifiers
  void notifyData(void);
  void notifyLayout(void);

 private:
};

#endif  // TOPOLOGYMODEL_H

#ifndef FRAMETABLEMODEL_H
#define FRAMETABLEMODEL_H

#include <QAbstractTableModel>
#include <PRIME/PrimeFrame.h>
#include <QDateTime>
#include <QVector>
#include "PRIMEProcessor.h"

class FrameTableModel : public QAbstractTableModel
{
  Q_OBJECT

  QVector<Frame> *frameList;
  int lastSize = 0;

  void colorizeRow(int, QColor const &);

public:
  FrameTableModel(QObject *parent, QVector<Frame> *);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void refreshData(void);

  static void registerTypes(void);
};

#endif // FRAMETABLEMODEL_H
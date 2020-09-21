#ifndef CREDENTIALMODEL_H
#define CREDENTIALMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <App/MeterInfo.h>

class CredInfoTableModel : public QAbstractTableModel
{
  Q_OBJECT

  QVector<CredInfo> *credInfoList;
  int lastSize = 0;

public:
  CredInfoTableModel(QObject *, QVector<CredInfo> *);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void appendData(QVector<CredInfo> const &);
};

#endif // CREDENTIALMODEL_H

#ifndef DLMSTABLEMODEL_H
#define DLMSTABLEMODEL_H

#include <QAbstractTableModel>
#include <PRIME/PrimeFrame.h>
#include <QDateTime>
#include <QVector>
#include <App/DLMSProcessor.h>

class DLMSTableModel : public QAbstractTableModel
{
  Q_OBJECT

  QVector<DlmsMessage> *messageList;
  int lastSize = 0;

public:
  DLMSTableModel(QObject *parent, QVector<DlmsMessage> *);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void clearData(void);
  void appendData(QVector<DlmsMessage> const &);
};

#endif // DLMSTABLEMODEL_H

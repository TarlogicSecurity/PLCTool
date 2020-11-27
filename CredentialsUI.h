#ifndef CREDENTIALSUI_H
#define CREDENTIALSUI_H

#include <QWidget>
#include <QDateTime>
#include <Topology/Concentrator.h>
#include <App/MeterInfo.h>

class QSortFilterProxyModel;
class CredInfoTableModel;

namespace Ui {
  class CredentialsUI;
}

class CredentialsUI : public QWidget
{
  Q_OBJECT

  QVector<CredInfo> credList;
  QVector<CredInfo> pendingList;

  QSortFilterProxyModel *proxy = nullptr;
  CredInfoTableModel *model = nullptr;

  bool sortingEnabled = false;
  bool adjusting = false;

  void saveLog(QString path);
  void connectAll(void);

public:
  explicit CredentialsUI(QWidget *parent = 0);
  ~CredentialsUI();

  void saveCreds(CredInfo const &);
  void refreshCredentials(void);
  void setSortingEnabled(bool);
  void clear(void);
  void realize(void);

public slots:
  void onSaveAs(bool);
  void onClear(bool);

private:
  Ui::CredentialsUI *ui;
};

#endif // CREDENTIALSUI_H

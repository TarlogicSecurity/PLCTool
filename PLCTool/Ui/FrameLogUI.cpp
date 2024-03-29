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

#include "PLCTool/Ui/FrameLogUI.h"

#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <cctype>
#include <cstdio>
#include <cstring>

#include "PLCTool/PRIME/PrimeAdapter.h"
#include "PLCTool/PRIME/PrimeFrame.h"
#include "ui_FrameLogUI.h"

Q_DECLARE_METATYPE(QVector<uint8_t>)
static bool typesRegistered = false;

FrameLogUI::FrameLogUI(QWidget *parent)
    : QWidget(parent), ui(new Ui::FrameLogUI)
{
  ui->setupUi(this);

  this->model = new FrameTableModel(this, &this->frameList);
  this->proxy = new QSortFilterProxyModel(this);

  this->proxy->setSourceModel(this->model);
  this->setSortingEnabled(this->sortingEnabled);
  this->proxy->sort(1);
  this->connectAll();

  this->savedHtml = this->ui->hexEdit->toHtml();
}

void
FrameLogUI::setSortingEnabled(bool enabled)
{
  this->sortingEnabled = enabled;
  this->ui->frameView->setModel(
      enabled ? static_cast<QAbstractItemModel *>(this->proxy)
              : static_cast<QAbstractItemModel *>(this->model));
  this->ui->frameView->setSortingEnabled(enabled);
}

void
FrameLogUI::saveLog(QString path)
{
  FILE *fp = nullptr;

  if ((fp = fopen(path.toStdString().c_str(), "w")) == nullptr) {
    QMessageBox::critical(
        this,
        "Failed to save frame log",
        "Cannot open " + path + " for writing: " + strerror(errno));
  } else {
    int i;

    for (i = 0; i < this->frameList.count(); ++i) {
      Frame *f = &this->frameList[i];

      fprintf(
          fp,
          "%d,%s,%c,%d,%s,%06x,%d,",
          i + 1,
          f->SNA.toStdString().c_str(),
          f->downlink ? 'D' : 'U',
          f->timeStamp.toTime_t(),
          f->frame != nullptr ? f->frame->typeToString().c_str() : "Corrupt",
          (f->frame != nullptr &&
           f->frame->PDU.macType == PLCTool::PrimeFrame::GENERIC)
              ? f->frame->PDU.PKT.LNID
              : 0xffffff,
          f->bytes.size());

      for (int j = 0; j < f->bytes.size(); ++j)
        fprintf(fp, "%02x", f->bytes[j]);

      fprintf(fp, "\n");
    }

    fclose(fp);
  }
}

void
FrameLogUI::connectAll(void)
{
  connect(
      this->ui->frameView,
      SIGNAL(activated(const QModelIndex &)),
      this,
      SLOT(onCellActivated(const QModelIndex &)));

  connect(
      this->ui->frameView,
      SIGNAL(clicked(const QModelIndex &)),
      this,
      SLOT(onCellActivated(const QModelIndex &)));

  connect(
      this->ui->frameView->selectionModel(),
      SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
      this,
      SLOT(onCurrentChanged(QModelIndex, QModelIndex)));

  connect(
      this->ui->clearButton,
      SIGNAL(clicked(bool)),
      this,
      SLOT(onClear(bool)));

  connect(
      this->ui->saveButton,
      SIGNAL(clicked(bool)),
      this,
      SLOT(onSaveAs(bool)));

  connect(this->ui->topButton, SIGNAL(clicked(bool)), this, SLOT(onTop()));

  connect(
      this->ui->bottomButton,
      SIGNAL(clicked(bool)),
      this,
      SLOT(onBottom()));

  connect(
      this->ui->gotoButton,
      SIGNAL(clicked(bool)),
      this,
      SLOT(onGotoLine()));
}

void
FrameLogUI::clear(void)
{
  this->onClear(false);
}

void
FrameLogUI::realize(void)
{
  this->proxy->sort(0, Qt::AscendingOrder);
}

void
FrameLogUI::saveFrame(Frame const &frame)
{
  this->pendingList.append(frame);
}

void
FrameLogUI::refreshFrames(void)
{
  if (this->pendingList.size() > 0) {
    int rows;
    bool oldAdjusting = this->adjusting;
    bool firstIter = this->frameList.size() == 0;
    this->adjusting = true;
    this->model->appendData(this->pendingList);
    this->pendingList.clear();

    rows = this->frameList.size();

    this->ui->lineSpin->setMinimum(rows > 0 ? 1 : 0);
    this->ui->lineSpin->setMaximum(rows);
    this->ui->lineSpin->setEnabled(rows > 0);
    this->ui->gotoButton->setEnabled(rows > 0);
    this->ui->topButton->setEnabled(rows > 0);
    this->ui->bottomButton->setEnabled(rows > 0);

    if (this->ui->autoScrollButton->isChecked())
      this->ui->frameView->scrollToBottom();

    if (firstIter)
      this->ui->frameView->resizeColumnsToContents();

    this->adjusting = oldAdjusting;
  }
}

void
FrameLogUI::registerTypes(void)
{
  if (!typesRegistered) {
    qRegisterMetaType<QVector<uint8_t>>();
    typesRegistered = true;
  }
  FrameTableModel::registerTypes();
}

void
FrameLogUI::selectNear(
    QDateTime const &,
    PLCTool::PrimeFrame::GenericType,
    PLCTool::NodeId)
{
}

FrameLogUI::~FrameLogUI()
{
  delete ui;
}

//////////////////////////////////// Slots ////////////////////////////////////
void
FrameLogUI::onCellActivated(const QModelIndex &index)
{
  Frame *frame = nullptr;
  int row = index.row();

  if (!this->adjusting && index.isValid()) {
    if (row >= 0 && row < this->ui->frameView->model()->rowCount()) {
      QModelIndex trueIndex =
          this->sortingEnabled ? this->proxy->mapToSource(index) : index;

      int ndx = this->model->data(trueIndex, Qt::UserRole).value<int>();

      if (ndx >= 0 && ndx < this->frameList.count())
        frame = &this->frameList[ndx];
    }

    if (frame != nullptr) {
      emit frameSelected(*frame);
      this->ui->hexEdit->setHtml(frame->toHtml());
      this->ui->pktEdit->setText(
          frame->frame != nullptr
              ? QString::fromStdString(frame->frame->toString())
              : "");
    } else {
      this->ui->hexEdit->setHtml(this->savedHtml);
      this->ui->pktEdit->setText("");
    }
  }
}

void
FrameLogUI::onCurrentChanged(QModelIndex curr, QModelIndex)
{
  onCellActivated(curr);
}

void
FrameLogUI::onSaveAs(bool)
{
  QFileDialog dialog(this);

  dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
  dialog.setNameFilter("Packet log (*.log)");
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  if (dialog.exec()) {
    QStringList list = dialog.selectedFiles();

    if (list.size() > 0)
      this->saveLog(list.at(0));
  }
}

void
FrameLogUI::onClear(bool)
{
  this->pendingList.clear();
  this->model->clearData();
  this->refreshFrames();
  this->ui->hexEdit->setHtml(this->savedHtml);
  this->ui->pktEdit->setText("");
  this->ui->lineSpin->setMinimum(0);
  this->ui->lineSpin->setMaximum(0);
  this->ui->lineSpin->setEnabled(false);
  this->ui->gotoButton->setEnabled(false);
  this->ui->topButton->setEnabled(false);
  this->ui->bottomButton->setEnabled(false);
}

void
FrameLogUI::onTop(void)
{
  this->ui->frameView->scrollToTop();
}

void
FrameLogUI::onBottom(void)
{
  this->ui->frameView->scrollToBottom();
}

void
FrameLogUI::onGotoLine(void)
{
  if (!this->adjusting)
    this->ui->frameView->scrollTo(this->ui->frameView->model()->index(
        this->ui->lineSpin->value() - 1,
        0));
}

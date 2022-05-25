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

#include "MeterUI.h"

#include "PLCTool/PRIME/PrimeAdapter.h"
#include "XMLHighlighter.h"
#include "ui_MeterUI.h"

using namespace PLCTool;

MeterUI::MeterUI(QWidget *parent, MeterInfo *info)
    : QWidget(parent), info(info), ui(new Ui::MeterUI)
{
  SubNet *top;

  ui->setupUi(this);

  this->info = info;

  this->frameModel = new FrameTableModel(this, info->frameList());
  this->dlmsModel = new DLMSTableModel(this, info->messageList());
  this->credInfoModel = new CredInfoTableModel(this, info->credList());

  this->frameProxy = new QSortFilterProxyModel(this);
  this->frameProxy->setSourceModel(this->frameModel);

  this->dlmsProxy = new QSortFilterProxyModel(this);
  this->dlmsProxy->setSourceModel(this->dlmsModel);

  this->credInfoProxy = new QSortFilterProxyModel(this);
  this->credInfoProxy->setSourceModel(this->credInfoModel);

  this->ui->frameView->setModel(this->frameProxy);
  this->ui->dlmsMessageView->setModel(this->dlmsProxy);
  this->ui->credInfoView->setModel(this->credInfoProxy);

  top = info->meter()->top();

  if (top != nullptr && top->parent() != nullptr) {
    this->ui->snLabel->setText(
        QString::fromStdString(PrimeAdapter::idToSna(top->parent()->id())));
  } else {
    this->ui->snLabel->setText("N/A");
  }

  this->ui->nodeIdLabel->setText(
      QString().asprintf("%06llx", static_cast<quint64>(info->meter()->id())));
  this->ui->sidLabel->setText(QString().asprintf(
      "0x%llx",
      static_cast<quint64>(PRIME13_SID(info->meter()->id()))));
  this->ui->lnidLabel->setText(QString().asprintf(
      "0x%llx",
      static_cast<quint64>(PRIME13_LNID(info->meter()->id()))));

  this->highlighter = new XMLHighlighter(this->ui->xmlEdit->document());

  this->savedHtml = this->ui->hexEdit->toHtml();
  this->savedText = this->ui->xmlEdit->toPlainText();

  this->connectAll();
  this->connectMeterInfo();
  this->refreshViews();
}

MeterUI::~MeterUI()
{
  delete ui;
}

void
MeterUI::connectAll(void)
{
  connect(
      this->ui->frameView,
      SIGNAL(activated(const QModelIndex &)),
      this,
      SLOT(onFrameCellActivated(const QModelIndex &)));

  connect(
      this->ui->frameView,
      SIGNAL(clicked(const QModelIndex &)),
      this,
      SLOT(onFrameCellActivated(const QModelIndex &)));

  connect(
      this->ui->frameView->selectionModel(),
      SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
      this,
      SLOT(onFrameCurrentChanged(QModelIndex, QModelIndex)));

  connect(
      this->ui->dlmsMessageView,
      SIGNAL(activated(const QModelIndex &)),
      this,
      SLOT(onDlmsCellActivated(const QModelIndex &)));

  connect(
      this->ui->dlmsMessageView,
      SIGNAL(clicked(const QModelIndex &)),
      this,
      SLOT(onDlmsCellActivated(const QModelIndex &)));

  connect(
      this->ui->dlmsMessageView->selectionModel(),
      SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
      this,
      SLOT(onDlmsCurrentChanged(QModelIndex, QModelIndex)));
}

void
MeterUI::connectMeterInfo(void)
{
  connect(this->info, SIGNAL(frameReceived(Frame)), this, SLOT(onFrame(Frame)));

  connect(
      this->info,
      SIGNAL(messageReceived(DlmsMessage)),
      this,
      SLOT(onDlmsMessage(DlmsMessage)));

  connect(
      this->info,
      SIGNAL(credentialsFound(CredInfo)),
      this,
      SLOT(onCredInfo(CredInfo)));
}

void
MeterUI::refreshViews(void)
{
  if (this->info->pendingFrameList()->size() > 0) {
    bool firstIter = this->info->frameList()->size() > 0;

    this->frameModel->appendData(*this->info->pendingFrameList());
    this->info->commitFrames();

    if (firstIter)
      this->ui->frameView->resizeColumnsToContents();
  }

  if (this->info->pendingMessageList()->size() > 0) {
    bool firstIter = this->info->messageList()->size() > 0;

    this->dlmsModel->appendData(*this->info->pendingMessageList());
    this->info->commitMessages();

    if (firstIter)
      this->ui->dlmsMessageView->resizeColumnsToContents();
  }

  if (this->info->pendingCredList()->size() > 0) {
    bool firstIter = this->info->credList()->size() > 0;

    this->credInfoModel->appendData(*this->info->pendingCredList());
    this->info->commitCreds();

    if (firstIter)
      this->ui->credInfoView->resizeColumnsToContents();
  }

  if (this->isVisible()) {
    this->ui->frameView->scrollToBottom();
    this->ui->dlmsMessageView->scrollToBottom();
    this->ui->credInfoView->scrollToBottom();
  }

  if (!this->info->frameList()->empty())
    this->ui->lastActivityLabel->setText(
        this->info->frameList()->last().timeStamp.toString());

  if (this->info->meter()->name().size() > 0)
    this->ui->nameLabel->setText(
        QString::fromStdString(this->info->meter()->name()));

  this->ui->macLabel->setText(this->info->mac());
}

void
MeterUI::onFrame(Frame frame)
{
  this->ui->lastActivityLabel->setText(frame.timeStamp.toString());
}

void
MeterUI::onDlmsMessage(DlmsMessage message)
{
  this->ui->lastActivityLabel->setText(message.timeStamp.toString());
}

void
MeterUI::onCredInfo(CredInfo info)
{
  this->ui->lastActivityLabel->setText(info.timeStamp.toString());
}

void
MeterUI::onFrameCellActivated(const QModelIndex &index)
{
  Frame *frame = nullptr;
  int row = index.row();

  if (row >= 0 && row < this->frameProxy->rowCount()) {
    QModelIndex trueIndex = this->frameProxy->mapToSource(index);

    int ndx = this->frameModel->data(trueIndex, Qt::UserRole).value<int>();

    if (ndx >= 0 && ndx < this->info->frameList()->count())
      frame = &(*this->info->frameList())[ndx];
  }

  if (frame != nullptr) {
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

void
MeterUI::onFrameCurrentChanged(QModelIndex curr, QModelIndex)
{
  onFrameCellActivated(curr);
}

void
MeterUI::onDlmsCellActivated(QModelIndex const &index)
{
  DlmsMessage *msg = nullptr;
  int row = index.row();

  if (row >= 0 && row < this->dlmsProxy->rowCount()) {
    QModelIndex trueIndex = this->dlmsProxy->mapToSource(index);
    int ndx = this->dlmsModel->data(trueIndex, Qt::UserRole).value<int>();

    if (ndx >= 0 && ndx < this->info->messageList()->count())
      msg = &(*this->info->messageList())[ndx];
  }

  if (msg != nullptr) {
    this->ui->xmlEdit->setText(msg->toText());
  } else {
    this->ui->xmlEdit->setText(this->savedText);
  }
}

void
MeterUI::onDlmsCurrentChanged(QModelIndex curr, QModelIndex)
{
  onDlmsCellActivated(curr);
}

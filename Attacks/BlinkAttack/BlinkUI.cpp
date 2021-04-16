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

#include "BlinkUI.h"

#include "ui_BlinkUI.h"
#include "util/defs.h"

BlinkUI::BlinkUI(QWidget *parent) : QWidget(parent), ui(new Ui::BlinkUI)
{
  ui->setupUi(this);

  setInfoStatus("Awaiting input");
  setIdleState();
  connectAll();
}

BlinkUI::~BlinkUI()
{
  delete ui;
}

void
BlinkUI::connectAll(void)
{
  connect(
        this->ui->startPushButton,
        SIGNAL(pressed()),
        this,
        SIGNAL(start(void)));

  connect(
        this->ui->cancelPushButton,
        SIGNAL(pressed()),
        this,
        SIGNAL(stop(void)));

  connect(
        this->ui->authenticationLevelComboBox,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(onChangedAuthenticationLevel(void)));
}

bool
BlinkUI::setIdleState(void)
{
  ui->inputFrame->setEnabled(true);
  ui->controlFrame->setEnabled(true);
  ui->startPushButton->setEnabled(true);
  ui->cancelPushButton->setEnabled(false);
  ui->progressBar->setEnabled(false);
  updatePasswordField();
  this->currentState = IDLE;

  return true;
}

bool
BlinkUI::setWaitingState(void)
{
  if (this->currentState != WAITING) {
    ui->inputFrame->setEnabled(false);
    ui->controlFrame->setEnabled(false);
    this->currentState = WAITING;
  }

  return true;
}

bool
BlinkUI::setRunningState(void)
{
  bool ok = this->currentState == WAITING;
  if (ok) {
    ui->inputFrame->setEnabled(false);
    ui->controlFrame->setEnabled(true);
    ui->startPushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(true);
    ui->progressBar->setEnabled(true);
    this->currentState = RUNNING;
  }

  return ok;
}

void
BlinkUI::setInfoStatus(const QString &message)
{
  QPalette messagePalette;
  messagePalette.setColor(QPalette::WindowText, Qt::black);
  ui->messageLabel->setPalette(messagePalette);
  ui->messageLabel->setText(message);
}

void
BlinkUI::setErrorStatus(const QString &message)
{
  QPalette messagePalette;
  messagePalette.setColor(QPalette::WindowText, Qt::red);
  ui->messageLabel->setPalette(messagePalette);
  ui->messageLabel->setText(message);
}

void
BlinkUI::setProgress(float progress)
{
  ui->progressBar->setValue(progress * this->ui->progressBar->maximum());
}

int
BlinkUI::getProgress(void) const
{
  return ui->progressBar->value();
}

void
BlinkUI::updatePasswordField(void)
{
  if (getAuthenticationLevel() == 0) {
    ui->passwordLabel->setEnabled(false);
    ui->passwordComboBox->setEnabled(false);
  } else {
    ui->passwordLabel->setEnabled(true);
    ui->passwordComboBox->setEnabled(true);
  }
}

std::vector<uint8_t>
BlinkUI::getSNA(void) const
{
  QString snaString = ui->snaComboBox->currentText();
  size_t snaLength = 6;
  std::vector<uint8_t> sna;

  if (snaString.length()) {
    snaString.remove(QRegularExpression("[-:., ]"));
    sna = fromHexStringToByteArray(snaString.toStdString(), snaLength);
  }

  return sna;
}

NodeId
BlinkUI::getNID(void) const
{
  QString nidString = ui->nidComboBox->currentText();
  NodeId nid;

  TRY(ulongFromString(nid, nidString));

  return nid;

fail:
  return (NodeId) -1;
}

ConnId
BlinkUI::getLCID(void) const
{
  QString lcidString = ui->lcidComboBox->currentText();
  ConnId lcid;

  TRY(ulongFromString(lcid, lcidString));

  return lcid;

fail:
  return (ConnId) -1;
}

uint8_t
BlinkUI::getLevel(void) const
{
  bool ok;
  uint8_t level = ui->levelComboBox->currentText().toUShort(&ok);

  if(!ok) level = (uint8_t) -1;

  return level;
}

DLMS_AUTHENTICATION
BlinkUI::getAuthenticationLevel(void) const
{
  return localAuthToDLMSAuth(ui->authenticationLevelComboBox->currentIndex());
}

std::string
BlinkUI::getPassword(void) const
{
  return ui->passwordComboBox->currentText().toStdString();
}

int
BlinkUI::getMsTimeout(void) const
{
  return ui->timeoutComboBox->currentText().toFloat() * 60000;
}

bool
BlinkUI::checkSNA(void)
{
  bool ok = !getSNA().empty();

  if (!ok) {
    setErrorStatus("SNA is required");
    ui->snaComboBox->setFocus();
  }

  return ok;
}

bool
BlinkUI::checkNID(void)
{
  bool ok = getNID() != (NodeId) -1;

  if (!ok) {
    setErrorStatus("NID is required");
    ui->nidComboBox->setFocus();
  }

  return ok;
}

bool
BlinkUI::checkLCID(void)
{
  bool ok = getLCID() != (ConnId) -1;

  if (!ok) {
    setErrorStatus("LCID is required");
    ui->lcidComboBox->setFocus();
  }

  return ok;
}

bool
BlinkUI::checkLevel(void)
{
  bool ok = getLevel() != (uint8_t) -1;

  if (!ok) {
    setErrorStatus("Level is required");
    ui->levelComboBox->setFocus();
  }

  return ok;
}

bool
BlinkUI::checkPassword(void)
{
  bool ok =
      getAuthenticationLevel() == DLMS_AUTHENTICATION_NONE ||
      !getPassword().empty();

  if (!ok) {
    setErrorStatus("Password is required");
    ui->passwordComboBox->setFocus();
  }

  return ok;
}

bool
BlinkUI::checkTimeout(void)
{
  bool ok = getMsTimeout() >= 1000;

  if (!ok) {
    setErrorStatus("A minimum timeout of 1s is required");
    ui->timeoutComboBox->setFocus();
  }

  return ok;
}

bool
BlinkUI::checkInput(void)
{
  return checkPassword() && checkTimeout() && checkSNA() && checkNID() &&
         checkLCID() && checkLevel();
}

bool
BlinkUI::ulongFromString(ulong &number, QString numberString) const
{
  bool ok;
  QRegularExpression reAF("[a-fA-F].");
  QRegularExpression re0x("0x|x");

  if (
      numberString.contains(reAF) ||
      numberString.indexOf(re0x) == 0)
    number = numberString.toULong(&ok, 16);
  else
    number = numberString.toULong(&ok);

  return ok;
}

std::vector<uint8_t>
BlinkUI::fromHexStringToByteArray(
    const std::string hexString,
    size_t byteArrayLength) const
{
  std::vector<uint8_t> byteArray;
  uint8_t byteBuffer[2];
  unsigned long i;
  unsigned long j;

  if (!byteArrayLength) {
    byteArrayLength = hexString.length() / 2;
    if (hexString.size() % 2) ++byteArrayLength;
  }

  i = 0;
  j = 0;
  if (hexString.size() % 2) {
    TRY(fromHexCharToByte(byteBuffer[0], hexString[0]));
    byteArray.push_back(byteBuffer[0]);

    ++i;
    ++j;
  }

  while (i < byteArrayLength && j < hexString.length()) {
    TRY(fromHexCharToByte(byteBuffer[1], hexString[j]));
    TRY(fromHexCharToByte(byteBuffer[0], hexString[j + 1]));
    byteArray.push_back(byteBuffer[0] + byteBuffer[1] * 16);

    ++i;
    j += 2;
  }

  while (i < byteArrayLength) {
    byteArray.insert(byteArray.begin(), 0);

    ++i;
  }

  return byteArray;

fail:
  return std::vector<uint8_t>();
}

bool
BlinkUI::fromHexCharToByte(uint8_t &b, char c) const
{
  bool ok = true;

  if (47 < c && c < 58)
    b = c - 48;
  else if (64 < c && c < 71)
    b = c - 55;
  else if (96 < c && c < 103)
    b = c - 87;
  else
    ok = false;

  return ok;
}

DLMS_AUTHENTICATION
BlinkUI::localAuthToDLMSAuth(int authenticationLevel) const
{
  DLMS_AUTHENTICATION dlmsAuthentication;

  switch (authenticationLevel) {
    case 0:
      dlmsAuthentication = DLMS_AUTHENTICATION_NONE;
      break;
    case 1:
      dlmsAuthentication = DLMS_AUTHENTICATION_LOW;
      break;
    case 2:
      dlmsAuthentication = DLMS_AUTHENTICATION_HIGH;
      break;
    case 3:
      dlmsAuthentication = DLMS_AUTHENTICATION_HIGH_MD5;
      break;
    case 4:
      dlmsAuthentication = DLMS_AUTHENTICATION_HIGH_SHA1;
      break;
    case 5:
      dlmsAuthentication = DLMS_AUTHENTICATION_HIGH_GMAC;
      break;
    case 6:
      dlmsAuthentication = DLMS_AUTHENTICATION_HIGH_SHA256;
      break;
    case 7:
      dlmsAuthentication = DLMS_AUTHENTICATION_HIGH_ECDSA;
      break;
    default:
      dlmsAuthentication = DLMS_AUTHENTICATION_NONE;
  }

  return dlmsAuthentication;
}

///////////////////////////////// Slots ////////////////////////////////////////

void
BlinkUI::onChangedAuthenticationLevel(void)
{
  updatePasswordField();
}

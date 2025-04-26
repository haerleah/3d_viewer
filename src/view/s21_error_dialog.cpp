/**
 * @file s21_error_dialog.cpp
 * @brief Error pop-up menu implementation.
 */

#include "s21_error_dialog.h"

#include "./ui_s21_error_dialog.h"

namespace s21 {

ErrorDialog::ErrorDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::ErrorDialog) {
  ui->setupUi(this);
}

ErrorDialog::~ErrorDialog() { delete ui; }

void ErrorDialog::SetMessage(std::string message) {
  findChild<QLabel*>("label")->setText(QString(message.data()));
}

}  // namespace s21

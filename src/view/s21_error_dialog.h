/**
 * @file s21_error_dialog.h
 * @brief Error pop-up menu header file.
 */

#ifndef S21_ERROR_DIALOG_H
#define S21_ERROR_DIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class ErrorDialog;
}

namespace s21 {

/**
 * @brief Class that implements the logic of the menu.
 **/
class ErrorDialog : public QDialog
{
    Q_OBJECT
public:
    ErrorDialog(QWidget* parent = nullptr);
    ~ErrorDialog();

    /**
    * @brief Sets message of error dialog.
    * @param message Message of error dialog, that will be shown to user.
    **/
    void SetMessage(std::string message);
private:
    Ui::ErrorDialog* ui;
};

} // namespace s21

#endif // S21_ERROR_DIALOG_H

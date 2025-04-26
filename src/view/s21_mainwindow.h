/**
 * @file s21_mainwindow.h
 * @brief Main app window header file
 */

#ifndef S21_MAINWINDOW_H
#define S21_MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QSettings>
#include <string>
#include <array>
#include <cmath>
#include "s21_openGL_widget.h"
#include "s21_error_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

/**
 * @brief Class that implements the logic of the main window.
 **/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(OGLWidget& widget, QWidget *parent = nullptr);
    MainWindow() = delete;
    ~MainWindow();
private slots:
    void on_loadButton_clicked();

    void on_upScaleButton_clicked();

    void on_downScaleButton_clicked();

    void on_colorComboBox_currentIndexChanged(int index);

    void on_redColorSlider_sliderMoved(int value);

    void on_greenColorSlider_sliderMoved(int value);

    void on_blueColorSlider_sliderMoved(int value);

    void on_edgeComboBox_currentIndexChanged(int index);

    void on_vertexComboBox_currentIndexChanged(int index);

    void on_projectionComboBox_currentIndexChanged(int index);

    void on_thiknessBox_currentIndexChanged(int index);

    void on_thicknessSlider_sliderMoved(int position);

    void on_xOffsetBox_valueChanged(int arg1);

    void on_yOffsetBox_valueChanged(int arg1);

    void on_zOffsetBox_valueChanged(int arg1);

    void on_xRotBox_valueChanged(int arg1);

    void on_yRotBox_valueChanged(int arg1);

    void on_zRotBox_valueChanged(int arg1);

    void on_screenshotPushButton_clicked();

    void on_imageComboBox_currentIndexChanged(int index);

    void on_screencastPushButton_clicked();

private:
    Ui::MainWindow *ui;
    OGLWidget& openGLWidget; ///< Reference to the widget that handles openGL rendering.
    int currentColorIndex; ///< Index of color combo box in UI.
    int thiknessBoxIndex; ///< Index of thickness combo box in UI.
    int screenshotBoxIndex; ///< Index of screenshot type combo box in UI.
    int linesThickness; ///< Value of edge thikness slider in UI.
    int verticesThickness; ///< Value of vertex thikness slider in UI.
    int xOffset, yOffset, zOffset; ///< Value of offset spinbox for each axis.
    int xRot, yRot, zRot; ///< Value of rotation slider for each axis.
    int projectionType; ///< Index of projection type combo box.
    int lineStyle; ///< Index of edge style combo box.
    int vertexStyle; ///< Index of vertex style combo box.
    std::string currentFile; ///< Name of the currently processed file.
    // 0 - model color, 1 - background color, 2 - vertices color
    std::array<std::array<int, 3>, 3> colors; ///< Array of model, background, and vertex colors. The components of each array are a color, represented as R, G, and B components.

    /**
   * @brief Override method of resize event.
   * Resizes the openGL widget depending on the main window size
   * @param event Pointer to a resize event.
   **/
    void resizeEvent(QResizeEvent* event) override;

    /**
   * @brief Override method of key press event.
   * Handling controlling the model with keys.
   * @param event Pointer to a key press event.
   **/
    void keyPressEvent(QKeyEvent* event) override;

    /**
   * @brief Opens system-based file dialog.
   * After selecting the file, the obj file will be uploaded to the backend.
   * @param parent Parent widget of dialog.
   **/
    void OpenFileDialog(QWidget *parent);

    /**
   * @brief Saves current settings into the file.
   **/
    void SaveSettings() const;

    /**
   * @brief Uploads settings from the file.
   **/
    void LoadSettings();

    /**
   * @brief Checks the presence and correctness of settings.
   * @return true in case of correct settings, false otherwise.
   **/
    bool CheckSettings() const;
};

} // namespace s21

#endif // S21_MAINWINDOW_H

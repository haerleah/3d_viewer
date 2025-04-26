/**
 * @file s21_mainwindow.h
 * @brief Main app window implemetation.
 */

#include "s21_mainwindow.h"

#include "./ui_s21_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(OGLWidget& widget, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), openGLWidget(widget) {
  ui->setupUi(this);
  openGLWidget.setParent(this);
  openGLWidget.show();
  projectionType = 0;
  lineStyle = 0;
  vertexStyle = 0;
  currentColorIndex = 0;
  thiknessBoxIndex = 0;
  screenshotBoxIndex = 0;
  xOffset = yOffset = zOffset = 0;
  xRot = yRot = zRot = 0;
  if (CheckSettings() == true) {
    LoadSettings();
  } else {
    linesThickness = 1;
    verticesThickness = 1;
    colors[0] = {10, 10, 10};
    colors[1] = {0, 0, 0};
    colors[2] = {10, 10, 10};
  }
}

MainWindow::~MainWindow() {
  openGLWidget.setParent(nullptr);
  SaveSettings();
  delete ui;
}

void MainWindow::on_loadButton_clicked() {
  OpenFileDialog(this);
  findChild<QLabel*>("filenameLabel")->setText(QString(currentFile.data()));
}

void MainWindow::on_upScaleButton_clicked() { openGLWidget.ScaleModel(true); }

void MainWindow::on_downScaleButton_clicked() {
  openGLWidget.ScaleModel(false);
}

void MainWindow::resizeEvent(QResizeEvent*) {
  openGLWidget.resize(width() - 200, height());
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Left:
      findChild<QSpinBox*>("yRotBox")->setValue(yRot - 1);
      break;
    case Qt::Key_Right:
      findChild<QSpinBox*>("yRotBox")->setValue(yRot + 1);
      break;
    case Qt::Key_Up:
      findChild<QSpinBox*>("xRotBox")->setValue(xRot + 1);
      break;
    case Qt::Key_Down:
      findChild<QSpinBox*>("xRotBox")->setValue(xRot - 1);
      break;
    case Qt::Key_Q:
      findChild<QSpinBox*>("zRotBox")->setValue(zRot - 1);
      break;
    case Qt::Key_E:
      findChild<QSpinBox*>("zRotBox")->setValue(zRot + 1);
      break;
  }
}

void MainWindow::OpenFileDialog(QWidget* parent) {
  std::string tempFilename =
      QFileDialog::getOpenFileName(parent, "Открыть файл", "",
                                   "OBJ 3D-модель (*.obj)")
          .toStdString();
  bool success = false;
  if (!tempFilename.empty()) {
    try {
      openGLWidget.LoadModel(tempFilename);
      findChild<QLabel*>("edgesLabel")
          ->setText(QString(
              std::to_string(openGLWidget.GetUniqueEdgesCount()).data()));
      findChild<QLabel*>("verticesLabel")
          ->setText(QString(
              std::to_string(openGLWidget.GetVerticesCount() / 3).data()));
      success = true;
    } catch (const std::invalid_argument& e) {
      ErrorDialog errorDialog(this);
      errorDialog.SetMessage("В файле содержатся ошибки!");
      errorDialog.exec();
    } catch (const std::out_of_range& e) {
      ErrorDialog errorDialog(this);
      errorDialog.SetMessage("Выбранный файл пуст!");
      errorDialog.exec();
    } catch (const std::logic_error& e) {
      ErrorDialog errorDialog(this);
      errorDialog.SetMessage("Файл не выбран или отсутсвует!");
      errorDialog.exec();
    }
    if (success) {
      currentFile = tempFilename;
    }
  }
}

void MainWindow::SaveSettings() const {
  QSettings settings("./3D_viewer.ini", QSettings::IniFormat);
  settings.setValue("linesThickness", linesThickness);
  settings.setValue("verticesThickness", verticesThickness);
  settings.setValue("projectionType", projectionType);
  settings.setValue("lineStyle", lineStyle);
  settings.setValue("vertexStyle", vertexStyle);
  settings.setValue("ModelR", colors[0][0]);
  settings.setValue("ModelG", colors[0][1]);
  settings.setValue("ModelB", colors[0][2]);
  settings.setValue("bckgrR", colors[1][0]);
  settings.setValue("bckgrG", colors[1][1]);
  settings.setValue("bckgrB", colors[1][2]);
  settings.setValue("vertR", colors[2][0]);
  settings.setValue("vertG", colors[2][1]);
  settings.setValue("vertB", colors[2][2]);
}

void MainWindow::LoadSettings() {
  QSettings settings("./3D_viewer.ini", QSettings::IniFormat);
  linesThickness = settings.value("linesThickness").toInt();
  findChild<QSlider*>("thicknessSlider")->setValue(linesThickness);
  openGLWidget.SetLineWidth(settings.value("linesThickness").toFloat() /
                            1000.0f);

  verticesThickness = settings.value("verticesThickness").toInt();
  openGLWidget.SetVertexWidth(settings.value("verticesThickness").toFloat() /
                              250.0f);

  openGLWidget.SetProjectionType(settings.value("projectionType").toInt());
  projectionType = settings.value("projectionType").toInt();
  findChild<QComboBox*>("projectionComboBox")
      ->setCurrentIndex(settings.value("projectionType").toInt());

  openGLWidget.SetLineStyle(settings.value("lineStyle").toInt());
  findChild<QComboBox*>("edgeComboBox")
      ->setCurrentIndex(settings.value("lineStyle").toInt());

  openGLWidget.SetVertexStyle(settings.value("vertexStyle").toInt());
  findChild<QComboBox*>("vertexComboBox")
      ->setCurrentIndex(settings.value("vertexStyle").toInt());

  openGLWidget.SetModelRed(settings.value("ModelR").toFloat() / 10.0f);
  openGLWidget.SetModelGreen(settings.value("ModelG").toFloat() / 10.0f);
  openGLWidget.SetModelBlue(settings.value("ModelB").toFloat() / 10.0f);
  colors[0][0] = settings.value("ModelR").toInt();
  colors[0][1] = settings.value("ModelG").toInt();
  colors[0][2] = settings.value("ModelB").toInt();
  findChild<QSlider*>("redColorSlider")
      ->setValue(settings.value("ModelR").toInt());
  findChild<QSlider*>("greenColorSlider")
      ->setValue(settings.value("ModelG").toInt());
  findChild<QSlider*>("blueColorSlider")
      ->setValue(settings.value("ModelB").toInt());

  openGLWidget.SetBackgroundRed(settings.value("bckgrR").toFloat() / 10.0f);
  openGLWidget.SetBackgroundGreen(settings.value("bckgrG").toFloat() / 10.0f);
  openGLWidget.SetBackgroundBlue(settings.value("bckgrB").toFloat() / 10.0f);
  colors[1][0] = settings.value("bckgrR").toInt();
  colors[1][1] = settings.value("bckgrG").toInt();
  colors[1][2] = settings.value("bckgrB").toInt();

  openGLWidget.SetVerticesRed(settings.value("vertR").toFloat() / 10.0f);
  openGLWidget.SetVerticesGreen(settings.value("vertG").toFloat() / 10.0f);
  openGLWidget.SetVerticesBlue(settings.value("vertB").toFloat() / 10.0f);
  colors[2][0] = settings.value("vertR").toInt();
  colors[2][1] = settings.value("vertG").toInt();
  colors[2][2] = settings.value("vertB").toInt();
}

bool MainWindow::CheckSettings() const {
  QSettings settings("./3D_viewer.ini", QSettings::IniFormat);
  bool result = true;
  std::vector<std::string> settingsKeys = {
      "linesThickness", "verticesThickness",
      "projectionType", "lineStyle",
      "vertexStyle",    "ModelR",
      "ModelG",         "ModelB",
      "bckgrR",         "bckgrG",
      "bckgrB",         "vertR",
      "vertG",          "vertB"};
  for (auto key : settingsKeys) {
    result = result && settings.contains(key);
  }
  return result;
}

void MainWindow::on_redColorSlider_sliderMoved(int value) {
  if (currentColorIndex == 0) {
    openGLWidget.SetModelRed(static_cast<float>(value) / 10.0f);
  } else if (currentColorIndex == 1) {
    openGLWidget.SetBackgroundRed(static_cast<float>(value) / 10.0f);
  } else {
    openGLWidget.SetVerticesRed(static_cast<float>(value) / 10.0f);
  }
  colors[currentColorIndex][0] = value;
}

void MainWindow::on_greenColorSlider_sliderMoved(int value) {
  if (currentColorIndex == 0) {
    openGLWidget.SetModelGreen(static_cast<float>(value) / 10.0f);
  } else if (currentColorIndex == 1) {
    openGLWidget.SetBackgroundGreen(static_cast<float>(value) / 10.0f);
  } else {
    openGLWidget.SetVerticesGreen(static_cast<float>(value) / 10.0f);
  }
  colors[currentColorIndex][1] = value;
}

void MainWindow::on_blueColorSlider_sliderMoved(int value) {
  if (currentColorIndex == 0) {
    openGLWidget.SetModelBlue(static_cast<float>(value) / 10.0f);
  } else if (currentColorIndex == 1) {
    openGLWidget.SetBackgroundBlue(static_cast<float>(value) / 10.0f);
  } else {
    openGLWidget.SetVerticesBlue(static_cast<float>(value) / 10.0f);
  }
  colors[currentColorIndex][2] = value;
}

void MainWindow::on_colorComboBox_currentIndexChanged(int index) {
  currentColorIndex = index;
  findChild<QSlider*>("redColorSlider")->setValue(colors[currentColorIndex][0]);
  findChild<QSlider*>("greenColorSlider")
      ->setValue(colors[currentColorIndex][1]);
  findChild<QSlider*>("blueColorSlider")
      ->setValue(colors[currentColorIndex][2]);
}

void MainWindow::on_edgeComboBox_currentIndexChanged(int index) {
  openGLWidget.SetLineStyle(index);
  lineStyle = index;
}

void MainWindow::on_vertexComboBox_currentIndexChanged(int index) {
  openGLWidget.SetVertexStyle(index);
  vertexStyle = index;
}

void MainWindow::on_projectionComboBox_currentIndexChanged(int index) {
  openGLWidget.SetProjectionType(index);
  projectionType = index;
}

void MainWindow::on_thiknessBox_currentIndexChanged(int index) {
  if (index == 0) {
    findChild<QSlider*>("thicknessSlider")->setValue(linesThickness);
  } else {
    findChild<QSlider*>("thicknessSlider")->setValue(verticesThickness);
  }
  thiknessBoxIndex = index;
}

void MainWindow::on_thicknessSlider_sliderMoved(int position) {
  if (thiknessBoxIndex == 0) {
    linesThickness = position;
    openGLWidget.SetLineWidth(static_cast<float>(position) / 1000.0f);
  } else {
    verticesThickness = position;
    openGLWidget.SetVertexWidth(static_cast<float>(position) / 250.0f);
  }
}

void MainWindow::on_xRotBox_valueChanged(int arg1) {
  xRot = arg1;
  openGLWidget.SetRotation(xRot, yRot, zRot);
}

void MainWindow::on_yRotBox_valueChanged(int arg1) {
  yRot = arg1;
  openGLWidget.SetRotation(xRot, yRot, zRot);
}

void MainWindow::on_zRotBox_valueChanged(int arg1) {
  zRot = arg1;
  openGLWidget.SetRotation(xRot, yRot, zRot);
}

void MainWindow::on_xOffsetBox_valueChanged(int arg1) {
  xOffset = arg1;
  openGLWidget.SetOffset(xOffset / 100.0f, yOffset / 100.0f, zOffset / 100.0f);
}

void MainWindow::on_yOffsetBox_valueChanged(int arg1) {
  yOffset = arg1;
  openGLWidget.SetOffset(xOffset / 100.0f, yOffset / 100.0f, zOffset / 100.0f);
}

void MainWindow::on_zOffsetBox_valueChanged(int arg1) {
  zOffset = arg1;
  openGLWidget.SetOffset(xOffset / 100.0f, yOffset / 100.0f, zOffset / 100.0f);
}

void MainWindow::on_screenshotPushButton_clicked() {
  if (screenshotBoxIndex == 0) {
    openGLWidget.GrabJPEG();
  } else {
    openGLWidget.GrabBMP();
  }
}

void MainWindow::on_imageComboBox_currentIndexChanged(int index) {
  screenshotBoxIndex = index;
}

void MainWindow::on_screencastPushButton_clicked() { openGLWidget.RecordGIF(); }

}  // namespace s21

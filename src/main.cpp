#include <QApplication>
#include "model/s21_model_facade.h"
#include "controller/s21_controller.h"
#include "view/s21_mainwindow.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    s21::ModelFacade model;
    s21::Controller controller(model);
    s21::OGLWidget openGlWidget(controller);
    s21::MainWindow w(openGlWidget);
    w.show();
    return a.exec();
}

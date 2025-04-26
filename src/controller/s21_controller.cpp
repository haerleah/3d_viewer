/**
 * @file s21_controller.cpp
 * @brief Viewer controller implementation.
 */

#include "s21_controller.h"

s21::Controller::Controller(ModelFacade &modelFacade) : facade(modelFacade) {}

void s21::Controller::ParseObjFile(const std::string &fileName) {
  facade.LoadFile(fileName);
}

std::pair<const std::vector<GLfloat> &, const std::vector<GLuint> &>
s21::Controller::GetBuffersData() const {
  return facade.GetBuffersData();
}

s21::ViewerData s21::Controller::InteractModel(TransformationStrategy strategy,
                                               InputData data) {
  return facade.InteractModel(data, strategy);
}

int s21::Controller::GetUnqueEdgesCount() const {
  return facade.GetUnqueEdgesCount();
}

/**
 * @file s21_model_facade.cpp
 * @brief Model facade implementation.
 */

#include "s21_model_facade.h"

namespace s21 {

ModelFacade::ModelFacade() : loaderInstance(ObjLoader::Instance()) {
  viewerModel = new Model();
  transformationStrategyContext = new Context();
}

ModelFacade::~ModelFacade() {
  delete viewerModel;
  delete transformationStrategyContext;
}

void ModelFacade::LoadFile(std::string filename) {
  loaderInstance.ParseFile(filename);
  viewerModel->CreateBuffers();
  viewerModel->ResetToDefault();
}

std::pair<const std::vector<GLfloat> &, const std::vector<GLuint> &>
ModelFacade::GetBuffersData() const {
  return {viewerModel->GetVertices(), viewerModel->GetIndices()};
}

int ModelFacade::GetUnqueEdgesCount() const {
  return loaderInstance.GetUniqueEdgesCount();
}

ViewerData ModelFacade::InteractModel(const InputData &params,
                                      TransformationStrategy method) {
  glm::mat4 modelMatrix;
  switch (method) {
    case TransformationStrategy::Rotate:
      transformationStrategyContext->SetStrategy(new RotateStrategy());
      break;
    case TransformationStrategy::Scale:
      transformationStrategyContext->SetStrategy(new ScaleStrategy());
      break;
    case TransformationStrategy::Move:
      transformationStrategyContext->SetStrategy(new MoveStrategy());
      break;
  }
  viewerModel->SetProjection(params.projectionType, params.width,
                             params.height);
  modelMatrix =
      transformationStrategyContext->TransformModel(params, viewerModel);
  return {modelMatrix, viewerModel->GetVP().viewMatrix,
          viewerModel->GetVP().projectionMatrix};
}

}  // namespace s21

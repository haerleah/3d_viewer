/**
 * @file s21_controller.h
 * @brief Viewer controller header file.
 */

#ifndef S21_CONTROLLER_H
#define S21_CONTROLLER_H

#include "../model/s21_model_facade.h"

namespace s21 {

class Controller {
 public:
  Controller() = delete;
  Controller(ModelFacade& modelFacade);
  ~Controller() = default;

  /**
   * @brief Uploads an .obj file with the given name to the backend.
   * @param fileName Name of .obj file.
   **/
  void ParseObjFile(const std::string& fileName);

  /**
   * @brief Gets buffers data of loaded model.
   * @return Pair of vectors with vertices and indices.
   **/
  std::pair<const std::vector<GLfloat>&, const std::vector<GLuint>&>
  GetBuffersData() const;

  /**
   * @brief Main method of model interaction.
   * @param strategy Strategy of model interaction.
   * @param data Structure with data needed for backend.
   * @return Matrices of model, view and projection, modified by backend and
   *combined into one structure.
   **/
  ViewerData InteractModel(TransformationStrategy strategy, InputData data);

  /**
   * @brief Gets number of unique edges.
   * @return Number of unique edges.
   **/
  int GetUnqueEdgesCount() const;

 private:
  ModelFacade& facade;  ///< Reference to the facade through which interaction
                        ///< with the model occurs.
};

}  // namespace s21

#endif  // S21_CONTROLLER_H

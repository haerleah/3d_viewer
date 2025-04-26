/**
 * @file s21_model_facade.h
 * @brief Model facade header file.
 */

#ifndef S21_MODEL_FACADE_H
#define S21_MODEL_FACADE_H

#include "s21_model.h"
#include "s21_obj_loader.h"
#include "s21_transformation_strategy.h"

namespace s21 {

/**
 * @brief Enumeration for defining transformation strategies.
 */
enum TransformationStrategy {
  Rotate,  ///< Rotation.
  Scale,   ///< Scaling.
  Move     ///< Translation.
};

/**
 * @brief Structure for storing model viewing data.
 */
struct ViewerData {
  glm::mat4 modelMatrix;       ///< Model matrix.
  glm::mat4 viewMatrix;        ///< View matrix.
  glm::mat4 projectionMatrix;  ///< Projection matrix.
};

/**
 * @brief Facade for working with the model.
 */
class ModelFacade {
 public:
  ModelFacade();   ///< Constructor of the ModelFacade class.
  ~ModelFacade();  ///< Destructor of the ModelFacade class.

  /**
   * @brief Loads the model file.
   * @param filename The name of the model file to load.
   */
  void LoadFile(std::string filename);

  /**
   * @brief Gets the model buffer data.
   * @return A pair containing constant references to the vertex and index
   * vectors.
   */
  std::pair<const std::vector<GLfloat>&, const std::vector<GLuint>&>
  GetBuffersData() const;

  /**
   * @brief Gets the number of unique edges of the model.
   * @return The number of unique edges.
   */
  int GetUnqueEdgesCount() const;

  /**
   * @brief Interacts with the model by applying the specified transformation
   * strategy.
   * @param params Input parameters for the transformation.
   * @param method Transformation method (Rotate, Scale, Move).
   * @return Model viewing data.
   */
  ViewerData InteractModel(const InputData& params,
                           TransformationStrategy method);

 protected:
  ObjLoader& loaderInstance;  ///< Instance of the OBJ loader.
  Model* viewerModel;         ///< Pointer to the model for viewing.
  Context* transformationStrategyContext;  ///< Context for the transformation
                                           ///< strategy.
};

}  // namespace s21

#endif  // S21_MODEL_FACADE_H

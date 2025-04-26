/**
 * @file s21_transformation_strategy.h
 * @brief Model interaction header file.
 */

#ifndef S21_TRANSFORMATION_STRATEGY_H
#define S21_TRANSFORMATION_STRATEGY_H

#include "s21_model.h"
#include <cmath>

namespace s21 {

/**
 * @brief Structure for storing input data for model transformation.
 */
struct InputData {
    GLfloat xRotationAngle; ///< Rotation angle around the X axis.
    GLfloat yRotationAngle; ///< Absolute rotation angles around each axis. Angles should be in degrees (from -360 to 360).
    GLfloat zRotationAngle; ///< Rotation angle around the Z axis.
    GLfloat scale; ///< Relative change in the model's scale. For smooth scaling up/down, it is recommended to use values 0.9/1.1.
    GLfloat xMoveOffset; ///< Offset along the X axis.
    GLfloat yMoveOffset; ///< Offset along the Y axis. Recommended step: 0.01.
    GLfloat zMoveOffset; ///< Offset along the Z axis.
    ProjectionType projectionType; ///< Type of projection (orthogonal/perspective).
    int width; ///< Width of the widget in pixels.
    int height; ///< Height of the widget in pixels.
};

/**
 * @brief Abstract class for transformation strategy.
 */
class Strategy {
public:
    virtual ~Strategy() {} ///< Destructor.

    /**
     * @brief Applies transformation to the model.
     * @param params Input parameters for transformation.
     * @param transform Transformation matrices.
     * @return Model transformation matrix.
     */
    virtual glm::mat4 Transform(const InputData& params, TransformationMatrices& transform) = 0;
};

/**
 * @brief Context for managing transformation strategies.
 */
class Context {
public:
    Context() : transformStrategy(nullptr) {} ///< Constructor initializing the transformation strategy as nullptr.
    ~Context(); ///< Destructor.

    /**
     * @brief Sets the transformation strategy.
     * @param strategy Pointer to the transformation strategy.
     */
    void SetStrategy(Strategy* strategy);

    /**
     * @brief Applies transformation to the model.
     * @param params Input parameters for transformation.
     * @param viewerModel Pointer to the model for viewing.
     * @return Model transformation matrix.
     */
    glm::mat4 TransformModel(const InputData& params, Model* viewerModel);

private:
    Strategy* transformStrategy; ///< Pointer to the current transformation strategy.
};

/**
 * @brief Rotation strategy for the model.
 */
class RotateStrategy : public Strategy {
public:
    /**
     * @brief Applies rotation to the model.
     * @param params Input parameters for transformation.
     * @param transform Transformation matrices.
     * @return Model transformation matrix.
     */
    glm::mat4 Transform(const InputData& params, TransformationMatrices& transform) override;
};

/**
 * @brief Scaling strategy for the model.
 */
class ScaleStrategy : public Strategy {
public:
    /**
     * @brief Applies scaling to the model.
     * @param params Input parameters for transformation.
     * @param transform Transformation matrices.
     * @return Model transformation matrix.
     */
    glm::mat4 Transform(const InputData& params, TransformationMatrices& transform) override;
};

/**
 * @brief Moving strategy for the model.
 */
class MoveStrategy : public Strategy {
public:
    /**
     * @brief Applies moving to the model.
     * @param params Input parameters for transformation.
     * @param transform Transformation matrices.
     * @return Model transformation matrix.
     */
    glm::mat4 Transform(const InputData& params, TransformationMatrices& transform) override;
};

} // namespace s21

#endif // S21_TRANSFORMATION_STRATEGY_H

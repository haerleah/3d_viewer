/**
 * @file s21_openGL_widget.h
 * @brief Model rendering widget header file.
 */

#ifndef S21_OPENGL_WIDGET_H
#define S21_OPENGL_WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <thread>
#include <chrono>
#include "s21_gif_recorder.h"
#include "../controller/s21_controller.h"

namespace s21 {

/**
 * @brief Class inherited from QOpenGLWidget.
 * Stores and manages data necessary for proper rendering
 **/
class OGLWidget : public QOpenGLWidget, private QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    OGLWidget(s21::Controller& controller, QWidget* parent = nullptr);
    OGLWidget() = delete;
    ~OGLWidget() = default;

   /**
   * @brief Loads .obj model file.
   * @param filename Upload file name.
   **/
    void LoadModel(std::string filename);

    /**
   * @brief Scaling model methid.
   * @param positiveScale bool parameter, in case of truth, scales the model by 1.1
   * relative to the current scale or by 0.9 otherwise.
   **/
    void ScaleModel(bool positiveScale);

    /**
   * @brief Sets red component of model color.
   * @param value Value of color's red component, can range from 0 to 1.
   **/
    void SetModelRed(float value);

    /**
   * @brief Sets green component of model color.
   * @param value Value of color's green component, can range from 0 to 1.
   **/
    void SetModelGreen(float value);

    /**
   * @brief Sets blue component of model color.
   * @param value Value of color's blue component, can range from 0 to 1.
   **/
    void SetModelBlue(float value);

    /**
   * @brief Sets red component of background color.
   * @param value Value of color's red component, can range from 0 to 1.
   **/
    void SetBackgroundRed(float value);

    /**
   * @brief Sets green component of background color.
   * @param value Value of color's green component, can range from 0 to 1.
   **/
    void SetBackgroundGreen(float value);

    /**
   * @brief Sets blue component of background color.
   * @param value Value of color's blue component, can range from 0 to 1.
   **/
    void SetBackgroundBlue(float value);

    /**
   * @brief Sets red component of vertices color.
   * @param value Value of color's red component, can range from 0 to 1.
   **/
    void SetVerticesRed(float value);

    /**
   * @brief Sets green component of vertices color.
   * @param value Value of color's green component, can range from 0 to 1.
   **/
    void SetVerticesGreen(float value);

    /**
   * @brief Sets blue component of vertices color.
   * @param value Value of color's blue component, can range from 0 to 1.
   **/
    void SetVerticesBlue(float value);

    /**
   * @brief Sets how edges are displayed.
   * @param value 0 - full, 1 - dotted.
   **/
    void SetLineStyle(int value);

    /**
   * @brief Sets how vertices are displayed.
   * @param value 0 - vertices aren't displayed, 1 - vertices displayed as square, 2 - vertices displayed as circle.
   **/
    void SetVertexStyle(int value);
    /**
   * @brief Sets projection type.
   * @param value 0 - orthogonal projection, 1 - perspective projection.
   **/
    void SetProjectionType(int value);

    /**
   * @brief Sets thickness of model's edges.
   * @param value Thickness value.
   **/
    void SetLineWidth(float value);

    /**
   * @brief Sets thickness of model's vertices.
   * @param value Thickness value.
   **/
    void SetVertexWidth(float value);

    /**
   * @brief Sets the model rotation for each axis in absolute values.
   * @param x Rotation by X axis.
   * @param y Rotation by Y axis.
   * @param z Rotation by Z axis.
   **/
    void SetRotation(int x, int y, int z);

    /**
   * @brief Sets the model offset for each axis in absolute values.
   * @param x Offset by X axis.
   * @param y Offset by Y axis.
   * @param z Offset by Z axis.
   **/
    void SetOffset(float x, float y, float z);

    /**
   * @brief Method for taking a screenshot in .jpeg format.
   **/
    void GrabJPEG();

    /**
   * @brief Method for taking a screenshot in .bmp format.
   **/
    void GrabBMP();

    /**
   * @brief Method for making .gif screencast.
   * Creates a thread, that emits signal for grabbing framebuffer
   * each 100 milliseconds.
   **/
    void RecordGIF();

    /**
   * @brief Getter of unique edges count.
   * @return  Number of unique edges.
   **/
    int GetUniqueEdgesCount() const;

    /**
   * @brief Getter of vertices count.
   * @return  Number of vertices.
   **/
    int GetVerticesCount() const;
signals:
    /**
   * @brief A signal that triggers the capture of the current framebuffer.
   **/
    void GrabSignal();
private slots:
    /**
   * @brief Slot executed on the main thread.
   * Grabs framebuffer of QOpenGLWidget
   **/
    void GrabGIFImage();
private:
    /**
   * @brief Overrided method of openGL initialization.
   * Compiles shader programm.
   **/
    void initializeGL() override;

    /**
   * @brief Overrided method of openGL rendering.
   * Collects data, sends it to the controller and renders the received data.
   **/
    void paintGL() override;

    /**
   * @brief Сreates VBO and EBO in current openGL context.
   * Should be called immediately after loading the model.
   **/
    void InitializeBuffers();

    /**
   * @brief Thread method.
   * Emits GrabSignal() every 100 milliseconds.
   **/
    void ThreadGrabbing();

    Controller& viewerController; ///< Reference to viewert controler.
    GifRecorder recorder; ///< Instance of gif recorder.
    TransformationStrategy currentStrategy; ///< Current strategy, strategy that will be executed on the backend side.
    ProjectionType currentProjectionType; ///< Current projection type.
    QOpenGLShaderProgram shaderProgramm; ///< Shader programm. Contains compiled shaders that will be executed on the GPU.
    QOpenGLBuffer VBO; ///< Vertex buffer object. Contains coordinates of model's vertices.
    QOpenGLBuffer EBO; ///< Element buffer object. Contains indices needed for rendering.
    std::array<float, 3> modelColor; ///< Array of model color. Consists of 3 components: R, G and B.
    std::array<float, 3> backgroundColor; ///< Array of background color. Consists of 3 components: R, G and B.
    std::array<float, 3> verticesColor; ///< Array of vertex color. Consists of 3 components: R, G and B.
    float scale; ///< Scale value.
    float linesThickness; ///< Width of model edges.
    float verticesThikness; ///< Width of model verices.
    int linesStyle; ///< Current edges style.
    int verticesStyle; ///< Current vertices style.
    int xRot, yRot, zRot; ///< Rotation values by each axis.
    float xOffset, yOffset, zOffset; ///< Offset values by each axis.

};

} // namespace s21

#endif // S21_OPENGL_WIDGET_H

/**
 * @file s21_openGL_widget.cpp
 * @brief Model rendering widget implementation.
 */

#include "s21_openGL_widget.h"

namespace s21 {

OGLWidget::OGLWidget(s21::Controller& controller, QWidget* parent)
    : QOpenGLWidget(parent),
      viewerController(controller),
      EBO(QOpenGLBuffer::IndexBuffer) {
  scale = 1.0;
  linesStyle = 0;
  verticesStyle = 0;
  xRot = yRot = zRot = 0;
  xOffset = yOffset = zOffset = 0.0f;
  linesThickness = 0.001f;
  verticesThikness = 0.01f;
  modelColor = {1.0, 1.0, 1.0};
  verticesColor = {1.0, 1.0, 1.0};
  backgroundColor = {0.0, 0.0, 0.0};
  currentProjectionType = s21::ProjectionType::Orthogonal;
  currentStrategy = s21::TransformationStrategy::Rotate;
  QObject::connect(this, &OGLWidget::GrabSignal, this,
                   &OGLWidget::GrabGIFImage);
}

void OGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/shaders/transform_shader.vert");
  shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                         ":/shaders/geometry_shader.glsl");
  shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/shaders/color_shader.frag");
}

void OGLWidget::paintGL() {
  glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  InputData data;
  data.height = height();
  data.width = width();
  data.projectionType = currentProjectionType;
  data.xRotationAngle = xRot;
  data.yRotationAngle = yRot;
  data.zRotationAngle = zRot;
  data.xMoveOffset = xOffset;
  data.yMoveOffset = yOffset;
  data.zMoveOffset = zOffset;
  data.scale = scale;
  ViewerData output = viewerController.InteractModel(currentStrategy, data);

  shaderProgramm.bind();
  shaderProgramm.setUniformValue(
      "modelMatrix",
      QMatrix4x4(glm::value_ptr(output.modelMatrix)).transposed());
  shaderProgramm.setUniformValue(
      "viewMatrix", QMatrix4x4(glm::value_ptr(output.viewMatrix)).transposed());
  shaderProgramm.setUniformValue(
      "projectionMatrix",
      QMatrix4x4(glm::value_ptr(output.projectionMatrix)).transposed());
  shaderProgramm.setUniformValue("lineStyle", linesStyle);
  shaderProgramm.setUniformValue("lineWidth", linesThickness);
  shaderProgramm.setUniformValue("pointSize", verticesThikness);
  shaderProgramm.setUniformValue("drawPoints", verticesStyle);
  shaderProgramm.setUniformValue(
      "lineColor", QVector4D(modelColor[0], modelColor[1], modelColor[2], 1));
  shaderProgramm.setUniformValue(
      "pointColor",
      QVector4D(verticesColor[0], verticesColor[1], verticesColor[2], 1));

  glEnableClientState(GL_VERTEX_ARRAY);
  VBO.bind();
  glVertexPointer(3, GL_FLOAT, 0, nullptr);
  EBO.bind();
  glDrawElements(GL_LINES, viewerController.GetBuffersData().second.size(),
                 GL_UNSIGNED_INT, nullptr);
  EBO.release();
  VBO.release();
  glDisableClientState(GL_VERTEX_ARRAY);
  shaderProgramm.release();
  scale = 1.0;
}

void OGLWidget::InitializeBuffers() {
  if (!VBO.isCreated()) {
    VBO.create();
  }
  VBO.bind();
  VBO.allocate(
      viewerController.GetBuffersData().first.data(),
      viewerController.GetBuffersData().first.size() * sizeof(GLfloat));
  VBO.release();

  if (!EBO.isCreated()) {
    EBO.create();
  }
  EBO.bind();
  EBO.allocate(
      viewerController.GetBuffersData().second.data(),
      viewerController.GetBuffersData().second.size() * sizeof(GLuint));
  EBO.release();
}

void OGLWidget::ThreadGrabbing() {
  size_t imageCounter = 0;
  recorder.CreateGif("screencast.gif", 640, 480, 10, 5);
  while (imageCounter < 50) {
    emit GrabSignal();
    imageCounter++;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  recorder.CompleteGIF();
}

void OGLWidget::LoadModel(std::string filename) {
  viewerController.ParseObjFile(filename);
  InitializeBuffers();
  update();
}

void OGLWidget::ScaleModel(bool positiveScale) {
  if (positiveScale) {
    scale = 1.1;
  } else {
    scale = 0.9;
  }
  currentStrategy = s21::TransformationStrategy::Scale;
  repaint();
}

void OGLWidget::SetModelRed(float value) {
  modelColor[0] = value;
  repaint();
}

void OGLWidget::SetModelGreen(float value) {
  modelColor[1] = value;
  repaint();
}

void OGLWidget::SetModelBlue(float value) {
  modelColor[2] = value;
  repaint();
}

void OGLWidget::SetBackgroundRed(float value) {
  backgroundColor[0] = value;
  repaint();
}

void OGLWidget::SetBackgroundGreen(float value) {
  backgroundColor[1] = value;
  repaint();
}

void OGLWidget::SetBackgroundBlue(float value) {
  backgroundColor[2] = value;
  repaint();
}

void OGLWidget::SetVerticesRed(float value) {
  verticesColor[0] = value;
  repaint();
}

void OGLWidget::SetVerticesGreen(float value) {
  verticesColor[1] = value;
  repaint();
}

void OGLWidget::SetVerticesBlue(float value) {
  verticesColor[2] = value;
  repaint();
}

void OGLWidget::SetLineStyle(int value) {
  linesStyle = value;
  repaint();
}

void OGLWidget::SetVertexStyle(int value) {
  verticesStyle = value;
  repaint();
}

void OGLWidget::SetProjectionType(int value) {
  if (value == 0) {
    currentProjectionType = s21::ProjectionType::Orthogonal;
  } else {
    currentProjectionType = s21::ProjectionType::Frustum;
  }
  repaint();
}

void OGLWidget::SetLineWidth(float value) {
  linesThickness = value;
  repaint();
}

void OGLWidget::SetVertexWidth(float value) {
  verticesThikness = value;
  repaint();
}

void OGLWidget::SetRotation(int x, int y, int z) {
  xRot = x;
  yRot = y;
  zRot = z;
  currentStrategy = s21::TransformationStrategy::Rotate;
  repaint();
}

void OGLWidget::SetOffset(float x, float y, float z) {
  xOffset = x;
  yOffset = y;
  zOffset = z;
  currentStrategy = s21::TransformationStrategy::Move;
  repaint();
}

void OGLWidget::GrabJPEG() {
  QImage grabbedImage = grabFramebuffer();
  grabbedImage.save("screenshot.jpeg");
}

void OGLWidget::GrabBMP() {
  QImage grabbedImage = grabFramebuffer();
  grabbedImage.save("screenshot.bmp");
}

void OGLWidget::RecordGIF() {
  std::thread recorderThread(&OGLWidget::ThreadGrabbing, this);
  recorderThread.detach();
}

int OGLWidget::GetUniqueEdgesCount() const {
  return viewerController.GetUnqueEdgesCount();
}

int OGLWidget::GetVerticesCount() const {
  return viewerController.GetBuffersData().first.size();
}

void OGLWidget::GrabGIFImage() {
  repaint();
  recorder.AddImage(grabFramebuffer());
}

}  // namespace s21

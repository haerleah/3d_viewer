#ifndef S21_MODEL_TESTS
#define S21_MODEL_TESTS

#include <gtest/gtest.h>

#include "../model/s21_model.h"
#include "../model/s21_model_facade.h"
#include "../model/s21_obj_loader.h"
#include "../model/s21_transformation_strategy.h"

TEST(FileLoader, SuccessTest_1) {
  EXPECT_NO_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_1.obj"));
  EXPECT_EQ(s21::ObjLoader::Instance().GetVertices().size() / 3, 3);
  EXPECT_EQ(s21::ObjLoader::Instance().GetFaces().size(), 12);
}

TEST(FileLoader, SuccessTest_2) {
  EXPECT_NO_THROW(s21::ObjLoader::Instance().ParseFile(
      "test/test_files/test_file_1_1.obj"));
  EXPECT_EQ(s21::ObjLoader::Instance().GetVertices().size() / 3, 3);
  EXPECT_EQ(s21::ObjLoader::Instance().GetFaces().size(), 12);
}

TEST(FileLoader, NonExist) {
  EXPECT_THROW(s21::ObjLoader::Instance().ParseFile("file_does_not_exist.obj"),
               std::logic_error);
}

TEST(FileLoader, Empty) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_2.obj"),
      std::out_of_range);
}

TEST(FileLoader, WrongVertex) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_3.obj"),
      std::invalid_argument);
}

TEST(FileLoader, WrongFace_1) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_4.obj"),
      std::invalid_argument);
}

TEST(FileLoader, WrongFace_2) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_5.obj"),
      std::invalid_argument);
}

TEST(FileLoader, WrongFace_3) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_6.obj"),
      std::invalid_argument);
}

TEST(FileLoader, WrongFace_4) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_1_2.obj"),
      std::invalid_argument);
}

TEST(FileLoader, ScaleNCenters) {
  s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_7.obj");
  s21::Vertex centerVertex = s21::ObjLoader::Instance().GetCenters();
  EXPECT_EQ(centerVertex.X, 2.0);
  EXPECT_EQ(centerVertex.Y, 3.0);
  EXPECT_EQ(centerVertex.Z, 2.0);
  EXPECT_EQ(s21::ObjLoader::Instance().GetScaleFactor(), 0.25f);
}

TEST(FileLoader, NoFaces) {
  EXPECT_THROW(
      s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_8.obj"),
      std::invalid_argument);
}

TEST(TransformStrategy, SetRotateStrategy) {
  s21::Context transformContext;
  s21::Model model;
  s21::InputData input;
  input.xRotationAngle = 30.0f;
  input.yRotationAngle = 10.0f;
  input.zRotationAngle = -5.0f;
  transformContext.SetStrategy(new s21::RotateStrategy);
  glm::mat4 result = transformContext.TransformModel(input, &model);
  EXPECT_EQ(result, model.GetTransformMatrices().rotateMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().scaleMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().translateMatrix);
}

TEST(TransformStrategy, SetScaleStrategy) {
  s21::Context transformContext;
  s21::Model model;
  s21::InputData input;
  input.scale = 1.5f;
  transformContext.SetStrategy(new s21::ScaleStrategy);
  glm::mat4 result = transformContext.TransformModel(input, &model);
  EXPECT_NE(result, model.GetTransformMatrices().rotateMatrix);
  EXPECT_EQ(result, model.GetTransformMatrices().scaleMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().translateMatrix);
}

TEST(TransformStrategy, SetMoveStrategy) {
  s21::Context transformContext;
  s21::Model model;
  s21::InputData input;
  input.xMoveOffset = 30.0f;
  input.yMoveOffset = 10.0f;
  input.zMoveOffset = -5.0f;
  transformContext.SetStrategy(new s21::MoveStrategy);
  glm::mat4 result = transformContext.TransformModel(input, &model);
  EXPECT_NE(result, model.GetTransformMatrices().rotateMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().scaleMatrix);
  EXPECT_EQ(result, model.GetTransformMatrices().translateMatrix);
}

TEST(Model, GetVertices) {
  s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_7.obj");
  s21::Model model;
  model.CreateBuffers();
  std::vector<GLfloat> vertices = model.GetVertices();
  EXPECT_EQ(vertices.size(), 6);
  EXPECT_EQ(vertices[0], -0.25f);
  EXPECT_EQ(vertices[1], -0.25f);
  EXPECT_EQ(vertices[2], 0.25f);
  EXPECT_EQ(vertices[3], 0.25f);
  EXPECT_EQ(vertices[4], 0.25f);
  EXPECT_EQ(vertices[5], -0.25f);
}

TEST(Model, GetIndices) {
  s21::ObjLoader::Instance().ParseFile("test/test_files/test_file_7.obj");
  s21::Model model;
  model.CreateBuffers();
  std::vector<GLuint> indices = model.GetIndices();
  EXPECT_EQ(indices.size(), 4);
  EXPECT_EQ(indices[0], 0);
  EXPECT_EQ(indices[1], 1);
}

TEST(Model, GetVPMatrices) {
  s21::Model model;
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 testViewMatrix = glm::lookAt(cameraPos, targetPos, upVector);
  EXPECT_EQ(model.GetVP().projectionMatrix, glm::mat4(1.0f));
  EXPECT_EQ(model.GetVP().viewMatrix, testViewMatrix);
}

TEST(Model, SetProjection) {
  s21::Model model;
  model.SetProjection(s21::ProjectionType::Orthogonal, 800, 600);
  EXPECT_EQ(model.GetVP().projectionMatrix,
            glm::ortho(-0.5f * (800.0f / 600.0f), 0.5f * (800.0f / 600.0f),
                       -0.5f, 0.5f, 0.1f, 100.0f));
  model.SetProjection(s21::ProjectionType::Orthogonal, 600, 800);
  EXPECT_EQ(model.GetVP().projectionMatrix,
            glm::ortho(-0.5f, 0.5f, -0.5f / (600.0f / 800.0f),
                       0.5f / (600.0f / 800.0f), 0.1f, 100.0f));
  model.SetProjection(s21::ProjectionType::Frustum, 800, 600);
  EXPECT_EQ(
      model.GetVP().projectionMatrix,
      glm::perspective(glm::radians(45.0f), (800.0f / 600.0f), 0.75f, 100.0f));
}

TEST(ModelFacade, loadFile) {
  s21::ModelFacade facade;
  EXPECT_NO_THROW(facade.LoadFile("test/test_files/test_file_1.obj"));

  EXPECT_TRUE(s21::ObjLoader::Instance().GetVertices().size() != 0);
  EXPECT_TRUE(s21::ObjLoader::Instance().GetFaces().size() != 0);
}

TEST(ModelFacade, getBuffersData) {
  s21::ModelFacade facade;
  EXPECT_NO_THROW(facade.LoadFile("test/test_files/test_file_7.obj"));
  std::pair<const std::vector<GLfloat>&, const std::vector<GLuint>&> bufData =
      facade.GetBuffersData();
  std::vector<GLfloat> vertices = bufData.first;
  EXPECT_EQ(vertices.size(), 6);
  EXPECT_EQ(vertices[0], -0.25f);
  EXPECT_EQ(vertices[1], -0.25f);
  EXPECT_EQ(vertices[2], 0.25f);
  EXPECT_EQ(vertices[3], 0.25f);
  EXPECT_EQ(vertices[4], 0.25f);
  EXPECT_EQ(vertices[5], -0.25f);
  std::vector<GLuint> indices = bufData.second;
  EXPECT_EQ(indices.size(), 4);
  EXPECT_EQ(indices[0], 0);
  EXPECT_EQ(indices[1], 1);
}

TEST(ModelFacade, InteractRotate) {
  s21::ModelFacade facade;
  s21::Model model;
  s21::Context transformContext;
  facade.LoadFile("test/test_files/test_file_7.obj");
  s21::InputData input;
  input.projectionType = s21::ProjectionType::Orthogonal;
  input.width = 800;
  input.height = 600;
  input.xRotationAngle = 30.0f;
  input.yRotationAngle = 10.0f;
  input.zRotationAngle = -5.0f;
  s21::ViewerData resultData =
      facade.InteractModel(input, s21::TransformationStrategy::Rotate);
  transformContext.SetStrategy(new s21::RotateStrategy);
  transformContext.TransformModel(input, &model);
  glm::mat4 result = resultData.modelMatrix;
  EXPECT_EQ(result, model.GetTransformMatrices().rotateMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().scaleMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().translateMatrix);
}

TEST(ModelFacade, InteractScale) {
  s21::ModelFacade facade;
  s21::Model model;
  s21::Context transformContext;
  facade.LoadFile("test/test_files/test_file_7.obj");
  s21::InputData input;
  input.scale = 1.5f;
  input.projectionType = s21::ProjectionType::Orthogonal;
  input.width = 800;
  input.height = 600;
  s21::ViewerData resultData =
      facade.InteractModel(input, s21::TransformationStrategy::Scale);
  transformContext.SetStrategy(new s21::ScaleStrategy);
  transformContext.TransformModel(input, &model);
  glm::mat4 result = resultData.modelMatrix;
  EXPECT_NE(result, model.GetTransformMatrices().rotateMatrix);
  EXPECT_EQ(result, model.GetTransformMatrices().scaleMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().translateMatrix);
}

TEST(ModelFacade, InteractMove) {
  s21::ModelFacade facade;
  s21::Model model;
  s21::Context transformContext;
  facade.LoadFile("test/test_files/test_file_7.obj");
  s21::InputData input;
  input.xMoveOffset = 30.0f;
  input.yMoveOffset = 10.0f;
  input.zMoveOffset = -5.0f;
  input.projectionType = s21::ProjectionType::Orthogonal;
  input.width = 800;
  input.height = 600;
  s21::ViewerData resultData =
      facade.InteractModel(input, s21::TransformationStrategy::Move);
  transformContext.SetStrategy(new s21::MoveStrategy);
  transformContext.TransformModel(input, &model);
  glm::mat4 result = resultData.modelMatrix;
  EXPECT_NE(result, model.GetTransformMatrices().rotateMatrix);
  EXPECT_NE(result, model.GetTransformMatrices().scaleMatrix);
  EXPECT_EQ(result, model.GetTransformMatrices().translateMatrix);
}

TEST(ModelFacede, GetUniqueEdgesCount) {
  s21::ModelFacade facade;
  s21::Model model;
  s21::Context transformContext;
  facade.LoadFile("test/test_files/test_file_9.obj");
  EXPECT_EQ(facade.GetUnqueEdgesCount(), 3);
  EXPECT_EQ(facade.GetBuffersData().second.size(), 24);
}

#endif

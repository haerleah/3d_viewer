/**
 * @file s21_obj_loader.cpp
 * @brief .obj file loader implementation.
 */

#include "s21_obj_loader.h"

namespace s21 {

ObjLoader& ObjLoader::Instance() {
  static ObjLoader loaderInstance;
  return loaderInstance;
}

void ObjLoader::ParseFile(std::string objFilename) {
  ClearData();
  filename = objFilename;
  std::ifstream fileStream(filename);
  if (fileStream.is_open()) {
    std::string line;
    float minX = FLT_MAX, maxX = FLT_MIN;
    float minY = FLT_MAX, maxY = FLT_MIN;
    float minZ = FLT_MAX, maxZ = FLT_MIN;
    while (std::getline(fileStream, line)) {
      std::istringstream iss(line);
      std::string token;
      std::getline(iss, token, ' ');
      if (token == "v") {
        float x, y, z;
        if (iss >> x >> y >> z) {
          vertices.insert(vertices.end(), {x, y, z});
          minX = std::min(minX, x);
          minY = std::min(minY, y);
          minZ = std::min(minZ, z);
          maxX = std::max(maxX, x);
          maxY = std::max(maxY, y);
          maxZ = std::max(maxZ, z);
        } else {
          ClearData();
          throw std::invalid_argument("wrong data");
        }
      } else if (token == "f") {
        ParseFace(line);
      } else {
        continue;
      }
    }
    fileStream.close();
    if (vertices.empty()) {
      ClearData();
      throw std::out_of_range("empty file");
    } else {
      if (faces.empty()) {
        ClearData();
        throw std::invalid_argument("wrong data");
      } else {
        modelCenter.X = (minX + maxX) / 2.0f;
        modelCenter.Y = (minY + maxY) / 2.0f;
        modelCenter.Z = (minZ + maxZ) / 2.0f;
        scaleFactor =
            0.5f / std::max(maxX - minX, std::max(maxY - minY, maxZ - minZ));
      }
    }
  } else {
    ClearData();
    throw std::logic_error("file doesn't exist");
  }
  uniqueEdgesCount = uniqueEdges.size();
  uniqueEdges.clear();
}

const std::vector<GLfloat>& ObjLoader::GetVertices() const { return vertices; }

const std::vector<GLuint>& ObjLoader::GetFaces() const { return faces; }

const Vertex& ObjLoader::GetCenters() const { return modelCenter; }

int ObjLoader::GetUniqueEdgesCount() const { return uniqueEdgesCount; }

GLfloat ObjLoader::GetScaleFactor() const { return scaleFactor; }

ObjLoader::ObjLoader() : scaleFactor(0), modelCenter({0, 0, 0}) {}

void ObjLoader::ParseFace(const std::string& line) {
  std::istringstream iss(line);
  std::string token;
  std::vector<GLuint> face;
  while (std::getline(iss, token, ' ')) {
    if (token.empty()) {
      continue;
    }
    std::size_t pos = token.find('/');
    if (pos != std::string::npos) {
      GLint index;
      try {
        index = std::stoi(token.substr(0, pos));
      } catch (std::invalid_argument const& e) {
        ClearData();
        throw std::invalid_argument("wrong data");
      } catch (std::out_of_range const& e) {
        ClearData();
        throw std::invalid_argument("wrong data");
      }
      if (static_cast<size_t>(index) > vertices.size() || index < 0 ||
          static_cast<size_t>(log10(index) + 1) !=
              token.substr(0, pos).size()) {
        ClearData();
        throw std::invalid_argument("wrong data");
      } else {
        face.push_back(index - 1);
      }
    } else if (token != "\r" && token != "f") {
      GLint index;
      try {
        index = std::stoi(token);
      } catch (std::invalid_argument const& e) {
        ClearData();
        throw std::invalid_argument("wrong data");
      } catch (std::out_of_range const& e) {
        ClearData();
        throw std::invalid_argument("wrong data");
      }
      size_t indexSize = static_cast<size_t>(log10(index) + 1);
      size_t tokenSize = token.size();
      if (token.back() == '\r') {
        tokenSize -= 1;
      }
      if (static_cast<size_t>(index) > vertices.size() || index < 0 ||
          indexSize != tokenSize) {
        ClearData();
        throw std::invalid_argument("wrong data");
      } else {
        face.push_back(index - 1);
      }
    }
  }
  for (size_t i = 1; i < face.size(); ++i) {
    uniqueEdges.emplace(face[i - 1], face[i]);
  }
  for (size_t i = 1; i < face.size(); ++i) {
    faces.push_back(face[i - 1]);
    faces.push_back(face[i]);
  }
  faces.push_back(face.back());
  faces.push_back(face.front());
}

void ObjLoader::ClearData() {
  vertices.clear();
  faces.clear();
  uniqueEdges.clear();
  scaleFactor = 0;
  uniqueEdgesCount = 0;
  modelCenter = {0, 0, 0};
}

Edge::Edge(GLuint a, GLuint b) {
  if (a > b) {
    v1 = a;
    v2 = b;
  } else {
    v2 = a;
    v1 = b;
  }
}

bool Edge::operator<(const Edge& other) const {
  if (v1 != other.v1) {
    return v1 < other.v1;
  }
  return v2 < other.v2;
}

}  // namespace s21

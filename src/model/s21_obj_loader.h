/**
 * @file s21_obj_loader.h
 * @brief .obj file loader header file.
 */

#ifndef S21_OBJ_LOADER_H
#define S21_OBJ_LOADER_H

#include <GL/gl.h>
#include <float.h>

#include <cmath>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

/**
 * @brief Structure for representing a vertex.
 */
struct Vertex {
  GLfloat X;  ///< X coordinate of the vertex.
  GLfloat Y;  ///< Y coordinate of the vertex.
  GLfloat Z;  ///< Z coordinate of the vertex.
};

/**
 * @brief Structure for representing an edge consisting of two vertices.
 */
struct Edge {
  GLuint v1, v2;  ///< Indices of the two vertices forming the edge.

  /**
   * @brief Constructor for creating an edge.
   * @param a Index of the first vertex.
   * @param b Index of the second vertex.
   */
  Edge(GLuint a, GLuint b);

  /**
   * @brief Comparison operator for sorting edges.
   * @param other Another edge for comparison.
   * @return true if the current edge is less than the other; otherwise false.
   */
  bool operator<(const Edge& other) const;
};

/**
 * @brief Class for loading and parsing OBJ files.
 */
class ObjLoader {
 public:
  ObjLoader(const ObjLoader& other) = delete;  ///< Disable copying.
  ObjLoader(ObjLoader&& other) = delete;       ///< Disable moving.
  ObjLoader& operator=(const ObjLoader& other) =
      delete;  ///< Disable copy assignment.
  ObjLoader& operator=(ObjLoader&& other) =
      delete;              ///< Disable move assignment.
  ~ObjLoader() = default;  ///< Default destructor.

  /**
   * @brief Gets the instance of the ObjLoader class (singleton).
   * @return Reference to the ObjLoader instance.
   */
  static ObjLoader& Instance();

  /**
   * @brief Parses the OBJ file.
   * @param objFilename The name of the OBJ file to parse.
   */
  void ParseFile(std::string objFilename);

  /**
   * @brief Gets the vertices of the model.
   * @return Constant reference to the vector of vertices.
   */
  const std::vector<GLfloat>& GetVertices() const;

  /**
   * @brief Gets the faces of the model.
   * @return Constant reference to the vector of faces.
   */
  const std::vector<GLuint>& GetFaces() const;

  /**
   * @brief Gets the center of the model.
   * @return Constant reference to the Vertex structure representing the center
   * of the model.
   */
  const Vertex& GetCenters() const;

  /**
   * @brief Gets the number of unique edges.
   * @return The number of unique edges.
   */
  int GetUniqueEdgesCount() const;

  /**
   * @brief Gets the scaling factor.
   * @return The scaling factor.
   */
  GLfloat GetScaleFactor() const;

 private:
  ObjLoader();  ///< Constructor of the ObjLoader class.

  /**
   * @brief Parses a string representing a face.
   * @param line String containing face data.
   */
  void ParseFace(const std::string& line);

  /**
   * @brief Clears the model data.
   */
  void ClearData();

  std::string filename;           ///< Name of the OBJ file.
  std::vector<GLfloat> vertices;  ///< Vector for storing the model's vertices.
  std::vector<GLuint> faces;      ///< Vector for storing the model's faces.
  std::set<Edge> uniqueEdges;     ///< Set of unique edges.
  int uniqueEdgesCount;           ///< Number of unique edges.
  GLfloat scaleFactor;            ///< Scaling factor.
  Vertex modelCenter;             ///< Center of the model.
};

}  // namespace s21

#endif  // S21_OBJ_LOADER_H

#pragma once

#define NOMINMAX

#include <GL/gl3w.h>
#include <tiny_gltf.h>
#include <vector>

namespace Model {
struct MeshContext {
  std::vector<GLuint> vertexArrays;
};
struct Context {
  std::vector<GLuint> buffers;
  std::vector<MeshContext> meshes;
};

tinygltf::Model LoadFromFile(const std::string &path);

Context CreateContext(const tinygltf::Model &model);

void CleanUpContext(const Context &ctx);

void Draw(const tinygltf::Model &model, const Context &ctx);

} // namespace Model

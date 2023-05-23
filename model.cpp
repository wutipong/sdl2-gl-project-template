#include "model.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <spdlog/spdlog.h>
#include <vector>

namespace Model {
tinygltf::TinyGLTF loader;
}

tinygltf::Model Model::LoadFromFile(const std::string &path) {
  std::string err;
  std::string warn;

  tinygltf::Model model;

  loader.LoadASCIIFromFile(&model, &err, &warn, path);

  spdlog::info("Loading model file: {}", path);
  if (!err.empty()) {
    spdlog::error("Cannot load model file: {}", err);
  } else if (!warn.empty()) {
    spdlog::warn("Model loaded with warning: {}", warn);
  }

  return model;
}

Model::Context Model::CreateContext(const tinygltf::Model &model) {
  Model::Context ctx;

  ctx.buffers.resize(model.buffers.size());
  glGenBuffers(model.buffers.size(), ctx.buffers.data());

  for (size_t i = 0; i < model.buffers.size(); i++) {
    glBindBuffer(GL_ARRAY_BUFFER, ctx.buffers[i]);
    glBufferData(GL_ARRAY_BUFFER, model.buffers[i].data.size(), model.buffers[i].data.data(), GL_STATIC_DRAW);
  }

  ctx.meshes.resize(model.meshes.size());

  constexpr GLuint iPosition = 0;
  constexpr GLuint iNormal = 1;

  for (size_t i = 0; i < model.meshes.size(); i++) {
    ctx.meshes[i].vertexArrays.resize(model.meshes[i].primitives.size());
    glCreateVertexArrays(ctx.meshes[i].vertexArrays.size(), ctx.meshes[i].vertexArrays.data());

    for (size_t j = 0; j < ctx.meshes[i].vertexArrays.size(); j++) {
      glBindVertexArray(ctx.meshes[i].vertexArrays[j]);

      for (auto &[name, index] : model.meshes[i].primitives[j].attributes) {
        auto &accessor = model.accessors[index];

        GLuint atrributeIndex = -1;
        if (name == "POSITION") {
          atrributeIndex = iPosition;
        } else if (name == "NORMAL") {
          atrributeIndex = iNormal;
        } else {
          continue;
        }

        int size = 1;
        switch (accessor.type) {
        default:
          assert(0);
          break;

        case TINYGLTF_TYPE_SCALAR:
          size = 1;
          break;

        case TINYGLTF_TYPE_VEC2:
          size = 2;
          break;

        case TINYGLTF_TYPE_VEC3:
          size = 3;
          break;

        case TINYGLTF_TYPE_VEC4:
          size = 4;
          break;
        }

        glVertexAttribPointer(atrributeIndex, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE,
                              accessor.ByteStride(model.bufferViews[accessor.bufferView]),
                              reinterpret_cast<void *>(model.bufferViews[accessor.bufferView].byteOffset));

        glEnableVertexAttribArray(atrributeIndex);
      }
    }
  }

  return ctx;
}

void Model::CleanUpContext(const Model::Context &ctx) {
  glDeleteBuffers(ctx.buffers.size(), ctx.buffers.data());

  for (auto &mesh : ctx.meshes) {
    glDeleteVertexArrays(mesh.vertexArrays.size(), mesh.vertexArrays.data());
  }
}

void Model::Draw(const tinygltf::Model &model, const Context &ctx) {
  for (int i = 0; i < model.meshes.size(); i++) {
    auto &mesh = model.meshes[i];
    for (int j = 0; j < model.meshes[i].primitives.size(); j++) {
      glBindVertexArray(ctx.meshes[i].vertexArrays[j]);

      auto &primitive = mesh.primitives[j];
      auto &accessor = model.accessors[0]; // TODO: find the actual accessor for the model.
      auto &bufferView = model.bufferViews[accessor.bufferView];
      glBindBuffer(GL_ARRAY_BUFFER, ctx.buffers[bufferView.buffer]);

      auto &indicesAccessor = model.accessors[primitive.indices];
      auto &indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.buffers[indicesBufferView.buffer]);

      glDrawElements(GL_TRIANGLES, indicesAccessor.count, indicesAccessor.componentType,
                     reinterpret_cast<void *>(indicesBufferView.byteOffset + indicesAccessor.byteOffset));
    }
  }
}

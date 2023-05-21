#include "model.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <spdlog/spdlog.h>

namespace Model {
tinygltf::TinyGLTF loader;
}

tinygltf::Model Model::LoadFromFile(const std::string& path) {
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
#include "texture.hpp"

#include "io_util.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <spdlog/spdlog.h>
#include <vector>

GLuint Texture::Load(const std::string &path) {
  std::vector<unsigned char> data;
  IOUtiliy::LoadFile(path, data, std::ios::binary | std::ios::in);

  spdlog::info("Loading texture: {}.", path);

  int width;
  int height;
  int channels_in_file;

  auto imageData =
      stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &width, &height, &channels_in_file, 4);

  if (imageData == nullptr) {
    spdlog::error("Loading texture fails!: {}", path);
    return 0;
  }

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

  stbi_image_free(imageData);

  return texture;
}
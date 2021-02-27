#include "shader.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>

#include "io_util.hpp"

namespace shader {
GLuint Compile(const GLenum &type, const std::string &srcStr);
}

GLuint shader::FromSource(const std::string &code, const GLenum &shaderType) {
  spdlog::info("Creating shader: {}.", code);
  return Compile(shaderType, code);
}

GLuint shader::LoadSource(const std::string &path, const GLenum &type) {
  spdlog::info("Loading shader: {}.", path);

  std::string srcStr;
  LoadFile(path, srcStr);

  return Compile(type, srcStr);
}

GLuint shader::Compile(const GLenum &type, const std::string &srcStr) {
  auto shader = glCreateShader(type);
  auto src = srcStr.data();

  glShaderSource(shader, 1, (const GLchar **)&src, 0);
  glCompileShader(shader);

  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

  if (compileStatus == GL_FALSE) {
    spdlog::error("Shader compile failed.");
  }

  GLint logLength;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    std::string log;
    log.resize(logLength);

    glGetShaderInfoLog(shader, log.size(), &logLength, log.data());
    spdlog::debug("Shader Log {}.", log);
  }

  return shader;
}

#include "shader.hpp"

#include <spdlog/spdlog.h>

#include "io_util.hpp"

GLuint CompileShader(const std::string &path, const GLenum &type) {
  spdlog::info("Shader compile: {}.", path);
  std::string srcStr;
  LoadFile(path, srcStr);

  auto shader = glCreateShader(type);
  auto src = srcStr.data();

  glShaderSource(shader, 1, (const GLchar **)&src, 0);
  glCompileShader(shader);

  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

  if (compileStatus == GL_FALSE) {
    printf("Shader compile failed: %s\n", path.c_str());
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

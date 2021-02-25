#include "shader.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>

#include "io_util.hpp"

namespace shader {
Options options;
}

void shader::Init(const shader::Options &options) {
  shader::options = options;
  spdlog::info("Shader system init.");
  spdlog::info("\tShader source: {}", options.SourcePath);
  spdlog::info("\tShader cache: {}", options.CachePath);
}

GLuint shader::Load(const std::string &name, const GLenum &type) {
  spdlog::info("Loading shader: {}.", name);
  
  auto path = std::filesystem::current_path()
                  .append(shader::options.SourcePath)
                  .append(name);

  std::string srcStr;
  LoadFile(path.string(), srcStr);

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

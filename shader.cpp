#include "shader.hpp"

#include "io_util.hpp"

GLuint CompileShader(const std::string &path, const GLenum &type) {
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
  }
  GLint logLength;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    std::vector<char> log;
    log.resize(logLength);

    glGetShaderInfoLog(shader, log.size(), &logLength, log.data());
    printf("%s", log.data());
  }

  return shader;
}

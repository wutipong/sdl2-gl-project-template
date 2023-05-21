#pragma

#include <GL/gl3w.h>
#include <string>

namespace Shader {
GLuint FromSource(const std::string &code, const GLenum &shaderType);
GLuint LoadSource(const std::string &path, const GLenum &shaderType);

GLuint LoadBinary(const std::string &path, const GLenum &shaderType, const std::string &entryPoint = "main");

void LinkProgram(const GLint &program);
} // namespace Shader

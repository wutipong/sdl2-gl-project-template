#pragma

#include <GL/gl3w.h>
#include <string>

namespace shader {
GLuint FromSource(const std::string &code, const GLenum &shaderType);
GLuint LoadSource(const std::string &path, const GLenum &shaderType);

GLuint LoadBinary(const std::string &path, const GLenum &shaderType,
                  const GLenum &binaryFormat);
} // namespace shader

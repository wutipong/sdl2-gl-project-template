#pragma

#include <GL/gl3w.h>
#include <string>

namespace shader {
GLuint FromSource(const std::string &code, const GLenum &shaderType);
GLuint LoadSource(const std::string &path, const GLenum &shaderType);
} // namespace shader

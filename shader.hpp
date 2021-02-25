#pragma

#include <GL/gl3w.h>
#include <string>

namespace shader {

struct Options {
  std::string SourcePath = "shaders";
  std::string CachePath = "shader_cached";
};

void Init(const Options& options = {});
GLuint Load(const std::string &name, const GLenum &type);
} // namespace shader

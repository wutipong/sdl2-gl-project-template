#include "scene.hpp"

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <vector>

#include "ImGuizmo.h"
#include "io_util.hpp"
#include "shader.hpp"

namespace {
glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
glm::vec4 lightDir{0, -1.0f, 0.75f, 0};
float ambient = 0.10f;

GLuint program;
GLuint vertShader;
GLuint fragShader;

GLuint vao;
GLuint vbo;

auto view = glm::lookAt(glm::vec3{5, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
auto world = glm::identity<glm::mat4>();

// FIXME: UV is initailized as 0.
// clang-format off
constexpr float vertices[] = {
  /* position       **  normal         **  uv */
  -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
   1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f, 1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
  -1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
  -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
  -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  -1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};
// clang-format on

constexpr int componentPerVector = 3;
constexpr int componentPerVertex = 2 * componentPerVector;
constexpr int componentCount = sizeof(vertices) / sizeof(float);
constexpr int vertexCount = componentCount / componentPerVertex;

constexpr glm::vec4 ClearColor = {0.33f, 0.67f, 1.0f, 1.00f};

// Uniform locations.
constexpr GLint uColor = 0;
constexpr GLint uViewProjection = 1;
constexpr GLint uWorld = 2;
constexpr GLint uLightDirection = 3;
constexpr GLint uAmbientIntensity = 4;

} // namespace

void Scene::Init() {

  constexpr bool useBinary = false;
  if (useBinary) {
    vertShader = Shader::LoadBinary("shaders/shader.vert.spv", GL_VERTEX_SHADER);
    fragShader = Shader::LoadBinary("shaders/shader.frag.spv", GL_FRAGMENT_SHADER);
  } else {
    vertShader = Shader::LoadSource("shaders/shader.vert", GL_VERTEX_SHADER);
    fragShader = Shader::LoadSource("shaders/shader.frag", GL_FRAGMENT_SHADER);
  }

  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  Shader::LinkProgram(program);

  glUseProgram(program);

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  constexpr GLuint iPosition = 0;
  constexpr GLuint iNormal = 1;
  constexpr GLuint iTexCoord = 2;

  constexpr GLsizei stride = 8 * sizeof(float);

  auto attributePosition = [](auto pos) constexpr { return reinterpret_cast<void *>(sizeof(float) * pos); };

  glVertexAttribPointer(iPosition, 3, GL_FLOAT, GL_FALSE, stride, attributePosition(0));
  glVertexAttribPointer(iNormal, 3, GL_FLOAT, GL_FALSE, stride, attributePosition(3));
  glVertexAttribPointer(iTexCoord, 2, GL_FLOAT, GL_FALSE, stride, attributePosition(6));

  glEnableVertexAttribArray(iPosition);
  glEnableVertexAttribArray(iNormal);
  glEnableVertexAttribArray(iTexCoord);
}

void Scene::CleanUp() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

void Scene::DoFrame(const FrameContext &ctx) {
  glViewport(0, 0, ctx.windowWidth, ctx.windowHeight);

  glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  constexpr float horizontal_fov = glm::pi<float>() / 2.0f;
  const float aspect = (float)ctx.windowWidth / (float)ctx.windowHeight;

  auto projection = glm::perspective<float>(horizontal_fov, aspect, 0.1f, 1000.0f);
  auto projectView = projection * view;

  glBindVertexArray(vao);
  glUseProgram(program);

  auto normalized = glm::normalize(lightDir);

  glUniformMatrix4fv(uViewProjection, 1, false, glm::value_ptr(projectView));
  glUniformMatrix4fv(uWorld, 1, false, glm::value_ptr(world));
  glUniform4f(uColor, color.r, color.g, color.b, color.a);
  glUniform4f(uLightDirection, normalized.r, normalized.g, normalized.b, normalized.a);
  glUniform1f(uAmbientIntensity, ambient);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Scene::DoUI() {
  const ImGuiIO &io = ImGui::GetIO();

  const ImVec2 gizmoSize = {200, 200};
  const ImVec2 gizmoPosition = {io.DisplaySize.x - gizmoSize.x, io.DisplaySize.y - gizmoSize.y};

  ImGuizmo::BeginFrame();
  ImGuizmo::ViewManipulate(glm::value_ptr(view), 5, gizmoPosition, gizmoSize, 0x80000000);

  ImGui::Begin("Lighting");
  ImGui::ColorPicker4("Color", glm::value_ptr(color), ImGuiColorEditFlags_Float);
  ImGui::DragFloat3("Direction", glm::value_ptr(lightDir), 0.01f, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
  ImGui::DragFloat("Ambient", &ambient, 0.01f, 0.0f, 0.5f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
  ImGui::End();
}

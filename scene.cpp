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

#include "model.hpp"

namespace {
glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
glm::vec4 lightDir{0, -1.0f, 0.75f, 0};
float ambient = 0.10f;

GLuint program;
GLuint vertShader;
GLuint fragShader;

GLuint vao;
GLuint vbo;

size_t elementOffset;
GLsizei elementCount;
GLenum elementType;

auto view = glm::lookAt(glm::vec3{5, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
auto world = glm::identity<glm::mat4>();
constexpr glm::vec4 ClearColor = {0.33f, 0.67f, 1.0f, 1.00f};

// Uniform locations.
constexpr GLint uColor = 0;
constexpr GLint uViewProjection = 1;
constexpr GLint uWorld = 2;
constexpr GLint uLightDirection = 3;
constexpr GLint uAmbientIntensity = 4;

tinygltf::Model model;
tinygltf::TinyGLTF loader;

Model::Context modelCtx;
} // namespace

void Scene::Init() {
  vertShader = Shader::LoadBinary("shaders/shader.vert.spv", GL_VERTEX_SHADER);
  fragShader = Shader::LoadBinary("shaders/shader.frag.spv", GL_FRAGMENT_SHADER);

  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  Shader::LinkProgram(program);

  glUseProgram(program);

  model = Model::LoadFromFile("assets/Cube.gltf");
  modelCtx = Model::CreateContext(model);
}

void Scene::CleanUp() {

  Model::CleanUpContext(modelCtx);
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
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glUseProgram(program);

  auto normalized = glm::normalize(lightDir);

  glUniformMatrix4fv(uViewProjection, 1, false, glm::value_ptr(projectView));
  glUniformMatrix4fv(uWorld, 1, false, glm::value_ptr(world));
  glUniform4f(uColor, color.r, color.g, color.b, color.a);
  glUniform4f(uLightDirection, normalized.r, normalized.g, normalized.b, normalized.a);
  glUniform1f(uAmbientIntensity, ambient);

  Model::Draw(model, modelCtx);
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

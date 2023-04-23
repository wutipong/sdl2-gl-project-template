#include "scene.hpp"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>

#include "io_util.hpp"
#include "shader.hpp"

namespace {
glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
glm::vec4 lightDir{0, -1.0f, 0, 0};
float ambient = 0.10f;

GLuint program;
GLuint vertShader;
GLuint fragShader;

GLuint vao;
GLuint vbo;

// clang-format off
constexpr float vertices[] = {
  /* position       **  normal       */
  -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
  -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
   1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
  -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
   1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
  -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
  -1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
  -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
  -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
   1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
   1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
   1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
   1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
   1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
  -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
  -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
  -1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
   1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
};
// clang-format on

constexpr int componentPerVector = 3;
constexpr int componentPerVertex = 2 * componentPerVector;
constexpr int componentCount = sizeof(vertices) / sizeof(float);
constexpr int vertexCount = componentCount / componentPerVertex;

constexpr glm::vec4 ClearColor = {0.33f, 0.67f, 1.0f, 1.00f};
} // namespace

void Scene::Init() {
  vertShader = Shader::LoadSource("shaders/shader.vert", GL_VERTEX_SHADER);
  fragShader = Shader::LoadSource("shaders/shader.frag", GL_FRAGMENT_SHADER);

  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(sizeof(float) * 3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
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

  auto projection = glm::perspective(horizontal_fov, aspect, 0.1f, 1000.0f);
  auto view = glm::lookAt(glm::vec3{5, 5, 5}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
  auto projectView = projection * view;

  auto world = glm::identity<glm::mat4>();

  glBindVertexArray(vao);
  glUseProgram(program);

  auto uColor = glGetUniformLocation(program, "in_Color");
  auto uViewProjection = glGetUniformLocation(program, "in_ViewProjection");
  auto uWorld = glGetUniformLocation(program, "in_World");
  auto uLightDirection = glGetUniformLocation(program, "in_LightDirection");
  auto uAmbientIntensity = glGetUniformLocation(program, "in_AmbientIntensity");

  auto normalized = glm::normalize(lightDir);

  glUniformMatrix4fv(uViewProjection, 1, false, glm::value_ptr(projectView));
  glUniformMatrix4fv(uWorld, 1, false, glm::value_ptr(world));
  glUniform4f(uColor, color.r, color.g, color.b, color.a);
  glUniform4f(uLightDirection, normalized.r, normalized.g, normalized.b, normalized.a);
  glUniform1f(uAmbientIntensity, ambient);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Scene::DoUI() {
  ImGui::Begin("Lighting");
  ImGui::ColorPicker4("Color", glm::value_ptr(color), ImGuiColorEditFlags_Float);
  ImGui::DragFloat3("Direction", glm::value_ptr(lightDir), 0.01f, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
  ImGui::DragFloat("Ambient", &ambient, 0.01f, 0.0f, 0.5f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
  ImGui::End();
}

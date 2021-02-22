#include "scene.hpp"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>

#include "io_util.hpp"

void Scene::Init() {

  std::string vertShaderSrc;
  LoadFile("shaders/shader.vert", vertShaderSrc);
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  auto src = vertShaderSrc.data();
  glShaderSource(vertShader, 1, (const GLchar **)&src, 0);
  glCompileShader(vertShader);

  std::string fragShaderSrc;
  LoadFile("shaders/shader.frag", fragShaderSrc);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  src = fragShaderSrc.data();
  glShaderSource(fragShader, 1, (const GLchar **)&src, 0);
  glCompileShader(fragShader);

  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);

  // clang-format off
    float vertices[] = {
      -1.0f, -1.0f,
      1.0f, -1.0f,
      0.0f, 1.0f,
    };
  // clang-format on

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
}

void Scene::CleanUp() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

void Scene::DoFrame(SDL_Event &event) {
  glBindVertexArray(vao);
  glUseProgram(program);

  glUniform4f(0, color.r, color.g, color.b, color.a);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Scene::DoUI() {
  ImGui::Begin("Editor");
  ImGui::ColorPicker4("Color", glm::value_ptr(color),
                      ImGuiColorEditFlags_Float);
  ImGui::End();
}

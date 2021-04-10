#include "scene.hpp"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>
#include <magic_enum.hpp>

#include "io_util.hpp"
#include "shader.hpp"

void Scene::Init() {
  vertShader = shader::LoadSource("shaders/shader.vert", GL_VERTEX_SHADER);
  fragShader = shader::LoadSource("shaders/shader.frag", GL_FRAGMENT_SHADER);

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

  auto uniform = glGetUniformLocation(program, "in_Color");

  glUniform4f(uniform, color.r, color.g, color.b, color.a);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Scene::DoUI() {
  ImGui::Begin("Editor");
  ImGui::ColorPicker4("Color", glm::value_ptr(color),
                      ImGuiColorEditFlags_Float);
  ImGui::End();
}

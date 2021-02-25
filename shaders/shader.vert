#version 410 core

layout(location = 0) in vec2 in_Position;

uniform vec4 in_Color;

out vec4 ex_Color;

void main() {
  gl_Position = vec4(in_Position.x, in_Position.y, 1.0f, 1.0f);

  ex_Color = in_Color;
}

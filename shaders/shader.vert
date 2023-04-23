#version 410 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

uniform vec4 in_Color;
uniform mat4 in_ViewProjection;
uniform mat4 in_World;

out vec4 ex_Color;
out vec3 ex_Normal;

void main() {

  mat4 transform = in_ViewProjection * in_World;

  gl_Position = transform * vec4(in_Position, 1.0);

  ex_Normal = (in_World * vec4(in_Normal, 0.0)).xyz;
  ex_Color = in_Color;
}
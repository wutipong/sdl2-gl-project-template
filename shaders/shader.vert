#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

layout(location = 0) uniform vec4 in_Color;
layout(location = 1) uniform mat4 in_ViewProjection;
layout(location = 2) uniform mat4 in_World;
layout(location = 3) uniform vec4 in_LightDirection;
layout(location = 4) uniform float in_AmbientIntensity;

layout(location = 0) out vec3 ex_Normal;
layout(location = 1) out vec2 ex_TexCoord;

void main() {
  mat4 transform = in_ViewProjection * in_World;

  gl_Position = transform * vec4(in_Position, 1.0);

  ex_Normal = (in_World * vec4(in_Normal, 0.0)).xyz;
  ex_TexCoord = in_TexCoord;
}
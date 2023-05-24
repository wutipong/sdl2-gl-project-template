#version 450 core

layout(location = 0) in vec3 ex_Normal;

layout(location = 0) out vec4 fragColor;

layout(location = 0) uniform vec4 in_Color;
layout(location = 1) uniform mat4 in_ViewProjection;
layout(location = 2) uniform mat4 in_World;
layout(location = 3) uniform vec4 in_LightDirection;
layout(location = 4) uniform float in_AmbientIntensity;

uniform sampler2D textureSampler;

void main(void) {
  float intensity = dot(-ex_Normal, in_LightDirection.xyz);
  vec4 diffuse = clamp(vec4(intensity * in_Color.rgb, 1.0), 0.0, 1.0);
  vec4 ambient = clamp(in_AmbientIntensity * in_Color, 0.0, 1.0);

  fragColor = clamp(diffuse + ambient, 0.0, 1.0) * textureSampler(texUnit, ex_TexCoord);
}

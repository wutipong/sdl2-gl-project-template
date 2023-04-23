#version 410 core

in vec4 ex_Color;
in vec3 ex_Normal;
out vec4 fragColor;

uniform vec4 in_LightDirection;
uniform float in_AmbientIntensity;

void main(void) {
  float intensity = dot(-ex_Normal, in_LightDirection.xyz);
  vec4 diffuse = clamp(vec4(intensity * ex_Color.rgb, 1.0), 0.0, 1.0);
  vec4 ambient = clamp(in_AmbientIntensity * ex_Color, 0.0, 1.0);

  fragColor = clamp(diffuse + ambient, 0.0, 1.0);
}

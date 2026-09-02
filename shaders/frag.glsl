#version 300 es

precision mediump float;

out vec4 FragColor;
in vec3 ourColor;
in vec3 vNormal;

void main() {
  vec3 n = normalize(vNormal);
  vec3 l = normalize(vec3(0.5f, 0.7f, 1.0f));
  float d = max(dot(n, l), 0.0f);
  FragColor = vec4(ourColor * (0.2f + 0.8f * d), 1.0f);
}

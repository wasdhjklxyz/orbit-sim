#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aHue;

out vec3 ourColor;

uniform float uTime;

const float PERIOD = 20.0f;

// From https://gist.github.com/GODPUS/e95d5bac68942076103c
vec3 hue2rgb(float hue) {
    return clamp(
        abs(mod(hue * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
}

void main() {
  gl_Position = vec4(aPos, 1.0);
  ourColor = hue2rgb(fract(aHue - uTime / PERIOD));
}

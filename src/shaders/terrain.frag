#version 330 core
out vec4 FragColor;

in vec3 FragPos;

void main() {
  float height = FragPos.y + 0.5f;

  vec3 result = vec3(1.0f * height);
  FragColor = vec4(result,  1.0);
}

#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 cameraPos;

void main() {
//  FragColor = vec4(vec3(FragPos.y),  1.0);
  float t = FragPos.y;
  vec3 color;

  if (t <= 0.2) {
    color = vec3(0.0, 0.6, 1.0) * (t + 0.1) * 4;
  } else if (t >= 0.1 && t < 0.3) {
    color = vec3(0.804, 0.714, 0.486) * t * 4; // Sand
  } else if (t >= 0.3) {
    color = vec3(0.03, 0.4, 0.1) * min(0.7, t / 4 + 0.2) * 4; // Grass
  }  

  FragColor = vec4(color, 1.0);
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 displacement = aPos;

    float frequency = 1.0;
    float amplitude = 0.5;

    // Rotation angles for each layer (in radians)
    float angle1 = 0.0;          // No rotation for layer 1
    float angle2 = 3.14159 / 4.0; // 45 degrees for layer 2
    float angle3 = 3.14159 / 2.0; // 90 degrees for layer 3
    float angle4 = 3.14159 * 3.0 / 4.0; // 135 degrees for layer 4

    // Compute sine and cosine for each rotation angle
    vec2 cosSin1 = vec2(cos(angle1), sin(angle1));
    vec2 cosSin2 = vec2(cos(angle2), sin(angle2));
    vec2 cosSin3 = vec2(cos(angle3), sin(angle3));
    vec2 cosSin4 = vec2(cos(angle4), sin(angle4));

    // Rotate the xz coordinates for each layer
    vec2 xz = vec2(displacement.x, displacement.z);
    vec2 xz1 = vec2(xz.x * cosSin1.x + xz.y * cosSin1.y, -xz.x * cosSin1.y + xz.y * cosSin1.x);
    vec2 xz2 = vec2(xz.x * cosSin2.x + xz.y * cosSin2.y, -xz.x * cosSin2.y + xz.y * cosSin2.x);
    vec2 xz3 = vec2(xz.x * cosSin3.x + xz.y * cosSin3.y, -xz.x * cosSin3.y + xz.y * cosSin3.x);
    vec2 xz4 = vec2(xz.x * cosSin4.x + xz.y * cosSin4.y, -xz.x * cosSin4.y + xz.y * cosSin4.x);

    // Compute displacement for each layer using rotated coordinates
    float displacementLayer1 = sin(xz1.x * frequency * 1.0) * cos(xz1.y * frequency * 1.0) * amplitude;
    float displacementLayer2 = sin(xz2.x * frequency * 2.0) * cos(xz2.y * frequency * 2.0) * amplitude;
    float displacementLayer3 = sin(xz3.x * frequency * 4.0) * cos(xz3.y * frequency * 4.0) * amplitude;
    float displacementLayer4 = sin(xz4.x * frequency * 8.0) * cos(xz4.y * frequency * 8.0) * amplitude;

    // Apply the total displacement to the y component
    displacement.y += (displacementLayer1 + displacementLayer2 + displacementLayer3 + displacementLayer4) / 4;

    FragPos = vec3(model * vec4(displacement, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

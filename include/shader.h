#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:  
  // Define the RGB struct
  struct RGB {
    float r, g, b;
  };

  // Convert hex string (e.g. "#FF9933") to RGB
  static RGB convertHexToRGB(const std::string& hexValue) {
    RGB rgbColor;
    int r, g, b;

    // Skip # if present
    std::string hex = (hexValue[0] == '#') ? hexValue.substr(1) : hexValue;

    if (hex.length() != 6) {
      return {1.0f, 1.0f, 1.0f}; // default white
    }

    sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);

    rgbColor.r = r / 255.0f;
    rgbColor.g = g / 255.0f;
    rgbColor.b = b / 255.0f;

    return rgbColor;
  }
  
  // Helper function to compile a shader from a given type and source
  static int compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str(); // Load the source into src
    glShaderSource(shader, 1, &src, nullptr); // Set the source code of the shader to src
    glCompileShader(shader);

    // Error checking
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
      int length;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
      char* buffer = (char*)alloca(length * sizeof(char));
      glGetShaderInfoLog(shader, length, &length, buffer);
      std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? " vertex " : " fragment ") << "shader" << std::endl;
      std::cout << buffer << std::endl;

      // Cleanup shader
      glDeleteShader(shader);
      return 0;
    }

    return shader;
  }

  std::string loadSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      std::cerr << "Failed to open shader file: " << filepath << std::endl;
      return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }


public:
  static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);


    // linking
    glAttachShader(program, vs); // Attach vertex shader to the program
    glAttachShader(program, fs); // Attach fragment shader to the program
    glLinkProgram(program);
    glValidateProgram(program);


    // Cleanup the shaders
    glDeleteShader(vs);
    glDeleteShader(fs); 

    return program;
  }

  // Generate a 3D shader based on a specified hex color
  unsigned int colorToShader3D(const std::string& hexValue) {
    RGB rgbColor = convertHexToRGB(hexValue);

    const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 u_transform;\n"
      "void main()\n"
      "{\n"
      "    vec4 pos = vec4(aPos, 1.0);\n"
      "    gl_Position = u_transform * pos;\n"
      "}\n";

    // Create the fragment shader with the actual RGB values
    std::stringstream fragSrc;
    fragSrc << "#version 330 core\n"
      << "out vec4 FragColor;\n"
      << "void main()\n"
      << "{\n"
      << "    FragColor = vec4("
      << rgbColor.r << ", "
      << rgbColor.g << ", "
      << rgbColor.b << ", 1.0);\n"
      << "}\n";

    std::string fragString = fragSrc.str();
    const char* fragmentShaderSource = fragString.c_str();

    unsigned int shader = createShader(vertexShaderSource, fragmentShaderSource);
    return shader;
  }

  // Generate a 2D shader based on a specified hex color
  static unsigned int colorToShader2D(const std::string& hexValue) {
    RGB rgbColor = convertHexToRGB(hexValue);

    const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec2 aPos;\n"
      "void main()\n"
      "{\n"
      "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
      "}\n";

    // Create the fragment shader with the actual RGB values
    std::stringstream fragSrc;
    fragSrc << "#version 330 core\n"
      << "out vec4 FragColor;\n"
      << "void main()\n"
      << "{\n"
      << "    FragColor = vec4("
      << rgbColor.r << ", "
      << rgbColor.g << ", "
      << rgbColor.b << ", 1.0);\n"
      << "}\n";

    std::string fragString = fragSrc.str();
    const char* fragmentShaderSource = fragString.c_str();

    unsigned int shader = createShader(vertexShaderSource, fragmentShaderSource);
    return shader;
  }
};

#endif

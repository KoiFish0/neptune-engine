#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Part of the below shader class is from: https://learnopengl.com/Getting-started/Shaders
class Shader {
public:
  unsigned int ID;
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  Shader(const char* vertexPath, const char* fragmentPath)
  {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();		
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();			
    }
    catch (std::ifstream::failure& e)
    {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

  }
  // activate the shader
  // ------------------------------------------------------------------------
  void use() const
  { 
    glUseProgram(ID); 
  }
  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const
  {         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
  }
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const
  { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
  }
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const
  { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
  }
  // ------------------------------------------------------------------------
  void setVec2(const std::string &name, const glm::vec2 &value) const
  { 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
  }
  void setVec2(const std::string &name, float x, float y) const
  { 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
  }
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const
  { 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
  }
  void setVec3(const std::string &name, float x, float y, float z) const
  { 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
  }
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const
  { 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
  }
  void setVec4(const std::string &name, float x, float y, float z, float w) const
  { 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
  }
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const
  {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  // Public methods below are original

  static unsigned int compileShader(std::string vertexShader, std::string fragmentShader) {
    unsigned int ID;

    const char* vShaderCode = vertexShader.c_str();
    const char * fShaderCode = fragmentShader.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    Shader::checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    Shader::checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    Shader::checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return ID;
  }

  class phongShader {
  public:
    unsigned int vertex, fragment;

  private:
    std::string vertexCode = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec3 aTexCoord;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "out vec3 Normal;\n"
    "out vec3 FragPos;  \n"
    "out vec3 TexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "  Normal = mat3(transpose(model)) * aNormal;  \n"
    "  FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "  TexCoord = aTexCoord;\n"
    "}       \n";

    std::string fragmentCode = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "  \n"
    "in vec2 TexCoord;\n"
    "in vec3 Normal;  \n"
    "in vec3 FragPos;  \n"
    "\n"
    "struct Material {\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "    float shininess;\n"
    "}; \n"
    "\n"
    "struct Light {\n"
    "    vec3 position;\n"
    "  \n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "};\n"
    "\n"
    "uniform Light light;  \n"
    "  \n"
    "uniform Material material;\n"
    "\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 lightPos;  \n"
    "uniform vec3 viewPos;\n"
    "\n"
    "void main() {    \n"
    "  // ambient\n"
    "  vec3 ambient = light.ambient * material.ambient;\n"
    "\n"
    "  // diffuse \n"
    "  vec3 norm = normalize(Normal);\n"
    "  vec3 lightDir = normalize(lightPos - FragPos);\n"
    "  float diff = max(dot(norm, lightDir), 0.0);\n"
    "  vec3 diffuse  = light.diffuse * (diff * material.diffuse);\n"
    "\n"
    "  // specular\n"
    "  vec3 viewDir = normalize(viewPos - FragPos);\n"
    "  vec3 reflectDir = reflect(-lightDir, norm);  \n"
    "  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    "  vec3 specular = light.specular * (spec * material.specular);  \n"
    "\n"
    "  vec3 result = ambient + diffuse + specular;\n"
    "  FragColor = vec4(result, 1.0);\n"
    "}  \n";

    unsigned int shader = Shader::compileShader(vertexCode, fragmentCode);

    void setUniforms();
  };

private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  static void checkCompileErrors(GLuint shader, std::string type)
  {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
    else
  {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
  
  // Private methods below are original

};
#endif

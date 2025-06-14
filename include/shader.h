#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <string>

std::string loadShaderSource(const std::string&);

unsigned int createShader(const std::string&, const std::string&);

struct RGB {
  float r, g, b;
};

RGB colorConverter(std::string);

unsigned int colorToShader(const std::string& hexValue);

unsigned int colorToShader_Text(const std::string&); 

#endif

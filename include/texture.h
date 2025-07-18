#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

class Texture {
public:
  static unsigned int loadTextureRGB(const char *path, int slot) {
    unsigned int textureRGB;
    glGenTextures(1, &textureRGB);
    glActiveTexture(GL_TEXTURE0 + slot); // slots are defined as an int starting at GL_TEXTURE0 so adding an int to it brings it to a slot of 0 + int
    glBindTexture(GL_TEXTURE_2D, textureRGB);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return textureRGB;
  }

  static unsigned int loadTextureRGBA(const char *path, int slot) {
    unsigned int textureRGBA;
    glGenTextures(1, &textureRGBA);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureRGBA);

    // Some texture parameters
    // TODO: Allow selection of these parameters through this method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      std::cout << "Loaded texture: " << path << " (ID: " << textureRGBA << ")" << std::endl;
    } else {
      std::cout << "Failed to load texture: " << path << " (" << stbi_failure_reason() << ")" << std::endl;
    }
    stbi_image_free(data);

    // Check OpenGL errors
    while (GLenum error = glGetError()) {
      std::cout << "OpenGL Error in loadTextureRGBA: " << error << std::endl;
    }

    return textureRGBA;
  }
};

#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <GLFW/glfw3.h>

#include <globals.h>
#include <stb_image/stb_image.h>

/*
 * TextureType describes what the texture is used for. This is important for
 * loading 3D models properly. The GENERIC 'tag' is intended to be used by
 * developers and the rest are mainly for the model loader.
 */
enum TextureType {
  GENERIC,
  DIFFUSE,
  SPECULAR,
  NORMAL,
  HEIGHT_MAP
};

class Texture {
public:
  unsigned int texture;
  enum TextureType type;

  /* TextureType has no effect outside of loading models. Use GENERIC if unsure. */
  Texture(const char* path, int slot, bool containsAlpha, enum TextureType type) {
    this->type = type;
    glGenTextures(1, &texture);
    /*
     * Slots are defined as integers starting at GL_TEXTURE0 (0x84c0).
     * Incrementing GL_TEXTURE0 gives GL_TEXTURE#, where: # = the number that is
     * incremented by.
     */
    glActiveTexture(GL_TEXTURE0 + slot); 
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Set the texture wrapping/filtering options */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* Load and generate the texture */
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, containsAlpha ? GL_RGBA : GL_RGB, width, height, 0, containsAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      if (debugPrint == true) {
        std::cout << "Loaded texture: " << path << " (ID: " << texture << ")" << std::endl;
      }
    } else {
      std::cout << "Failed to load texture: " << path << " (" << stbi_failure_reason() << ")" << std::endl;
    }

    stbi_image_free(data);
  }
};

#endif 

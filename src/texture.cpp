#include "texture.h"

#include <glad/glad.h>
#ifdef __linux
#include <stb/stb_image.h>
#elif __APPLE__
#include "../include/stb_image.h"
#elif _WIN32
#include "stb_image.h"
#endif
#include <iostream>

Texture::Texture(const char* filePath, bool withAlpha) {
    filepath = filePath;
    loadWithAlpha = withAlpha;
    loadFromFile = true;
}

Texture::Texture(const unsigned char* buffer, const unsigned* size, bool withAlpha) {
    _buffer = buffer;
    _size = size;
    loadWithAlpha = withAlpha;
    loadFromFile = false;
}

unsigned int Texture::genAndBindAndLoad() {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int desiredChannels = loadWithAlpha ? STBI_rgb_alpha : STBI_rgb;
    unsigned char* imgData;
    if(loadFromFile)
        imgData = stbi_load(filepath, &width, &height, &nrChannels, desiredChannels);
    else
        imgData = stbi_load_from_memory(_buffer, *_size, &width, &height, &nrChannels, desiredChannels);

    // If successful, generate texture into the bound GL texture
    if (imgData) {
        // Generate texture and mipmaps
        if (loadWithAlpha)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture!" << std::endl;
    }

    // Free img data
    stbi_image_free(imgData);
    return ID;
}

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    unsigned int ID;

    Texture(const char* filePath, bool withAlpha);
    Texture(const unsigned char* buffer, const unsigned* size, bool withAlpha);
    unsigned int genAndBindAndLoad();
private:
    const char* filepath;
    const unsigned char* _buffer;
    const unsigned* _size;
    bool loadWithAlpha;
    bool loadFromFile;
    int width, height, nrChannels;
};

#endif

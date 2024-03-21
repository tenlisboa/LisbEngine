
#ifndef OPENGLBOOK_TEXTURE_H
#define OPENGLBOOK_TEXTURE_H

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


enum WrapParams {
    Repeat = GL_REPEAT,
    MirrorRepeat = GL_MIRRORED_REPEAT,
    ClampEdge = GL_CLAMP_TO_EDGE,
    ClampBorder = GL_CLAMP_TO_BORDER
};

enum FilterParams {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

class Texture {

public:


    unsigned int ID;
    unsigned int textureAddr;

    Texture() {
        glGenTextures(1, &ID);
        textureAddr = GL_TEXTURE0;
        bind();
    }

    Texture(int texAddrIndex) {
        glGenTextures(1, &ID);
        textureAddr = GL_TEXTURE0 + texAddrIndex;
        bind();
    }


    Texture& setWrapX(WrapParams param) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);

        return *this;
    }

    Texture& setWrapY(WrapParams param) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);

        return *this;

    }

    Texture& setMagFilter(FilterParams param) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);

        return *this;
    }


    Texture& setMinFilter(FilterParams param) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);

        return *this;
    }

    Texture& loadImage(std::string filename, unsigned int internalFormat, unsigned int format) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "ERROR::LOADING_TEXTURE\n" << std::endl;
        }
        stbi_image_free(data);

        return *this;
    }

    Texture& activate() {
        glActiveTexture(textureAddr);

        return *this;
    }

    Texture& bind() {
        glBindTexture(GL_TEXTURE_2D, ID);

        return *this;
    }

    Texture& use() {
        activate();
        bind();

        return *this;
    }
};

#endif //OPENGLBOOK_TEXTURE_H

#include "Texture.h"
#include "FreeImage.h"
#include <iostream>
#include "glm/glm.hpp"

using namespace std;

// create texture from file.
Texture::Texture(string filename)
{
    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image, once loaded
    FIBITMAP *dib(0);
    //pointer to the image data
    BYTE* bits(0);
    //image width and height
    unsigned int fiWidth(0), fiHeight(0);

    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(filename.c_str(), 0);
    //if still unknown, try to guess the file format from the file extension
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
    }
    //if still unkown, return failure
    if(fif == FIF_UNKNOWN) {
        cout << "error opening texture : " << filename << endl;
        return;
    }

    if(FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, filename.c_str());
    //if the image failed to load, return failure
    if(!dib) {
        cout << "error opening texture : " << filename << endl;
        return;
    }

    bits = FreeImage_GetBits(dib);
    unsigned int width = FreeImage_GetWidth(dib);
    unsigned int height = FreeImage_GetHeight(dib);

//    glCreateTextures(GL_TEXTURE_2D, 1, &glidTexture);
    unsigned int nbLevels;
    // this computes nbLevels = log_2(max(mSizeX,mSizeY)), which is the maximum number of
    // mipmap level we can have for this size.
//    nbLevels = 1;
//    unsigned int tempSize = glm::max(width, height);
//    while (tempSize >>= 1) ++nbLevels;

//    glTextureStorage2D(glidTexture, nbLevels, GL_RGB32F, width, height);
//        glGenTextures(1, &glidTexture);
//        glBindTexture(GL_TEXTURE_2D, glidTexture);
//    glTextureStorage2D(glidTexture, nbLevels, GL_RGB32F, width, height);
//    glClearTexImage(glidTexture, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
//    glTextureParameteri(glidTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTextureParameteri(glidTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTextureParameteri(glidTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTextureParameteri(glidTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTextureSubImage2D(glidTexture, 0, 0, 0, width, height,
//                        GL_BGR, GL_UNSIGNED_BYTE, bits);
//    glGenerateTextureMipmap(glidTexture);

    glGenTextures(1, &glidTexture);
    glBindTexture(GL_TEXTURE_2D, glidTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
//    glTexSubImage2D(glidTexture, 0, 0, 0, width, height,
//                        GL_BGR, GL_UNSIGNED_BYTE, bits);
//    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);


}

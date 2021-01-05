#include "texture.hpp"

#include<string>
#include<iostream>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

Texture::Texture()
{
    glGenTextures(1, &texture_id);
}

Texture::~Texture()
{
    std::cout << "kasjd" << std::endl;
    glDeleteTextures(1, &texture_id);
}

void Texture::load(const std::string& filename)
{
    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLenum gl_format;
    SDL_Surface* surface_texture = IMG_Load(filename.c_str());
    if(surface_texture == nullptr)
        std::cout << "IMG_Load error: "  << IMG_GetError() << std::endl;
    
    if(surface_texture->format->BytesPerPixel == 4)
    {
        if(surface_texture->format->Rmask == 0x000000FF)
            gl_format = GL_RGBA;
        else
            gl_format = GL_BGRA;
    }
    else
    {
        if(surface_texture->format->Rmask == 0x000000FF)
            gl_format = GL_RGB;
        else
            gl_format = GL_BGR;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, surface_texture->w, surface_texture->h, 0, gl_format, GL_UNSIGNED_BYTE, surface_texture->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    SDL_FreeSurface(surface_texture); // Free SDL_Surface memory
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

void Texture::start()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::stop()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

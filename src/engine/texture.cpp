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
    glDeleteTextures(1, &texture_id);
}

void Texture::set_texture_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Uint32 amask = 0xff000000;
    Uint32 bmask = 0x00ff0000;
    Uint32 gmask = 0x0000ff00;
    Uint32 rmask = 0x000000ff;

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 100, 100, 32, rmask, gmask, bmask, amask);
    

    SDL_LockSurface(surface);

    Uint32 color = SDL_MapRGBA(surface->format, r, g, b, a);
    Uint8* pixels = (Uint8*)surface->pixels;

    for(int i = 0; i<surface->w * surface->h; ++i)
    {
        SDL_memcpy(pixels+i*4, &color, 4);
    }

    SDL_UnlockSurface(surface);

    load_texture(surface);
    SDL_FreeSurface(surface); // Free SDL_Surface memory
}

void Texture::load_texture(SDL_Surface* surface_texture)
{
    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLenum gl_format;
    
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

    SDL_LockSurface(surface_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, surface_texture->w, surface_texture->h, 0, gl_format, GL_UNSIGNED_BYTE, surface_texture->pixels);

    SDL_UnlockSurface(surface_texture);    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

void Texture::load(const std::string& filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if(surface == nullptr)
        std::cout << "IMG_Load error: "  << IMG_GetError() << std::endl;

    load_texture(surface);
    SDL_FreeSurface(surface); // Free SDL_Surface memory
}

void Texture::start() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::stop() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

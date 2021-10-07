#pragma once

#include<string>

#include<GL/glew.h>
#include<SDL2/SDL.h>

class Texture
{
    public:
        Texture();
        ~Texture();

        void load(const std::string& filename);
        void set_texture_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
        void start() const;
        void stop() const;
    private:
        void load_texture(SDL_Surface* surface);
        GLuint texture_id;
};

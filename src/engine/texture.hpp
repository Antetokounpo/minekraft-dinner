#pragma once

#include<string>

#include<GL/glew.h>

class Texture
{
    public:
        Texture();
        ~Texture();

        void load(const std::string& filename);
        void start();
        void stop();
    private:
        GLuint texture_id;
};

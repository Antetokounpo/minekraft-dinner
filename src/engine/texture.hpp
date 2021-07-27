#pragma once

#include<string>

#include<GL/glew.h>

class Texture
{
    public:
        Texture();
        ~Texture();

        void load(const std::string& filename);
        void start() const;
        void stop() const;
    private:
        GLuint texture_id;
};

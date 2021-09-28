#pragma once

#include<string>
#include<vector>

#include<GL/glew.h>

#include "model.hpp"

class Skybox
{
    public:
        Skybox();
        ~Skybox();

        void load(const std::vector<std::string>& filenames);
        void start() const;
        void stop() const;
        unsigned get_vertex_count() const;
    private:
        GLuint texture_id;
        GLuint vao;
        GLuint vbo;
        unsigned vertex_count;
};

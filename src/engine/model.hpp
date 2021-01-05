#pragma once

#include<vector>
#include<string>

#include<GL/glew.h>

class Model
{
    public:
        Model();
        ~Model();
        void load(float vertices[], size_t s_vertices, float uvs[], size_t s_uvs, float normals[], size_t s_normals, int indices[], size_t s_indices);
        void render();
        void start();
        void stop();
        int get_vertex_count();
    private:
        void create_VBO(float data[], size_t s_data, unsigned int index, unsigned int coord_format);
        void create_EBO(int indices[], size_t s_indices);

        GLuint vao;
        GLuint ebo;
        std::vector<GLuint> vbos; 
        int vertex_count;
};

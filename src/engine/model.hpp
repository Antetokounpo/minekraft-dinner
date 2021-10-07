#pragma once

#include<vector>
#include<string>

#include<GL/glew.h>

class Model
{
    public:
        Model();
        ~Model();
        void load(const std::string& obj_filename);
        void load(float vertices[], size_t s_vertices, float uvs[], size_t s_uvs, float normals[], size_t s_normals, unsigned int indices[], size_t s_indices);
        void load(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned>& indices);
        void start() const;
        void stop() const;
        int get_vertex_count() const;
    private:
        Model(const Model& m1); // Copy constructor
        Model& operator=(const Model& m); // Copy assignement operator
        void create_VBO(float data[], size_t s_data, unsigned int index, unsigned int coord_format);
        void create_EBO(unsigned int indices[], size_t s_indices);

        GLuint vao;
        GLuint ebo;
        std::vector<GLuint> vbos; 
        int vertex_count;
};

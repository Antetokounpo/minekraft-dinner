#include "model.hpp"

#include<string>

#include<GL/glew.h>

#include<engine/loaders/objloader.hpp>

Model::Model()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
}

Model::~Model()
{
    glBindVertexArray(0); // Unbind any vao
    glDeleteVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for(GLuint& vbo: vbos)
        glDeleteBuffers(1, &vbo);
}

void Model::create_EBO(unsigned int indices[], size_t s_indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indices, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind buffer
}

void Model::create_VBO(float data[], size_t s_data, unsigned int index, unsigned int coord_format)
{
    vbos.push_back(0); // Declare new vbo
    GLuint& vbo = vbos.back();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, s_data, data, GL_STATIC_DRAW);
    glVertexAttribPointer(index, coord_format, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer
}

void Model::load(float vertices[], size_t s_vertices, float uvs[], size_t s_uvs, float normals[], size_t s_normals, unsigned int indices[], size_t s_indices)
{
    glBindVertexArray(vao);

    create_EBO(indices, s_indices);

    create_VBO(vertices, s_vertices, 0, 3);
    create_VBO(uvs, s_uvs, 1, 2);
    create_VBO(normals, s_normals, 2, 3);

    glBindVertexArray(0); // Unbind VAO and associated VBOs

    vertex_count = s_indices/sizeof(indices[0]);
}

void Model::load(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned>& indices)
{
    load(&vertices[0], vertices.size()*sizeof(vertices[0]), &uvs[0], uvs.size()*sizeof(uvs[0]), &normals[0], normals.size()*sizeof(normals[0]), &indices[0], indices.size()*sizeof(indices[0]));
}

void Model::load(const std::string& obj_filename)
{
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    OBJLoader::load_file(obj_filename, vertices, uvs, normals, indices);
    Model::load(&vertices[0], vertices.size()*sizeof(vertices[0]), &uvs[0], uvs.size()*sizeof(uvs[0]), &normals[0], normals.size()*sizeof(normals[0]), &indices[0], indices.size()*sizeof(indices[0]));
}

void Model::start() const
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Model::stop() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO
}

int Model::get_vertex_count() const
{
    return vertex_count;
}
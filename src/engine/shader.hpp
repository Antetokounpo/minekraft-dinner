#pragma once

#include<string>
#include<map>

#include<GL/glew.h>
#include<glm/glm.hpp>

class Shader
{
    public:
        Shader();
        ~Shader();

        void load(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
        void start();
        void stop();
        GLuint get_id();

        void set_uniform_variable(const float& v, const std::string& uniform_variable);
        void set_uniform_variable(const glm::vec3& v, const std::string& uniform_varible);
        void set_uniform_variable(const glm::mat4& v, const std::string& uniform_varible);
    private:
        std::string read_file(const std::string& filename);
        void compile_shader(const std::string& source, GLuint shader_id);
        void add_uniform_location(const std::string& uniform_variable);
        void init_uniform_vars();

        std::map<std::string, GLuint> uniform_var_dict;
        GLuint shader_program;
};
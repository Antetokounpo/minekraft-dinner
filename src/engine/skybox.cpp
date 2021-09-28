#include "skybox.hpp"

#include<iostream>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

Skybox::Skybox()
{
    glGenTextures(1, &texture_id);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    float size = 400.0f;
    std::vector<float> vertices = {        
	    -size,  size, -size,
	    -size, -size, -size,
	    size, -size, -size,
	     size, -size, -size,
	     size,  size, -size,
	    -size,  size, -size,

	    -size, -size,  size,
	    -size, -size, -size,
	    -size,  size, -size,
	    -size,  size, -size,
	    -size,  size,  size,
	    -size, -size,  size,

	     size, -size, -size,
	     size, -size,  size,
	     size,  size,  size,
	     size,  size,  size,
	     size,  size, -size,
	     size, -size, -size,

	    -size, -size,  size,
	    -size,  size,  size,
	     size,  size,  size,
	     size,  size,  size,
	     size, -size,  size,
	    -size, -size,  size,

	    -size,  size, -size,
	     size,  size, -size,
	     size,  size,  size,
	     size,  size,  size,
	    -size,  size,  size,
	    -size,  size, -size,

	    -size, -size, -size,
	    -size, -size,  size,
	     size, -size, -size,
	     size, -size, -size,
	    -size, -size,  size,
	     size, -size,  size
	};
    vertex_count = vertices.size();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer
    glBindVertexArray(0); // Unbind VAO and associated VBOs
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &texture_id);

    glBindVertexArray(0); // Unbind any vao
    glDeleteVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
}

void Skybox::load(const std::vector<std::string>& filenames)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    GLenum gl_format;
    SDL_Surface* surface_texture;
    for(unsigned i = 0; i<filenames.size(); ++i)
    {
        surface_texture = IMG_Load(filenames[i].c_str());
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

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, gl_format, surface_texture->w, surface_texture->h, 0, gl_format, GL_UNSIGNED_BYTE, surface_texture->pixels);

        SDL_FreeSurface(surface_texture); // Free SDL_Surface memory
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Pour "fusionner les bords de la skybox"
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // Unbind texture
}

void Skybox::start() const
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
}

void Skybox::stop() const
{
    glDisableVertexAttribArray(0);
    glBindVertexArray(0); // Unbind VAO

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned Skybox::get_vertex_count() const
{
    return vertex_count;
}

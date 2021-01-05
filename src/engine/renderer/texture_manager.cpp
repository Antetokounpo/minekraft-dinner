#include "texture_manager.hpp"

#include<memory>

#include<GL/glew.h>

#include<iostream> // debug

TextureManager::TextureManager(){}

TextureManager::~TextureManager(){}

void TextureManager::load_texture(const std::string& filename)
{
    std::unique_ptr<Texture> t(new Texture);
    t->load(filename);
    textures.push_back(std::move(t));
    std::cout << "alksdjlaks" << std::endl;
}

void TextureManager::start_texture(int id)
{
    textures[id]->start();
}

void TextureManager::stop_texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

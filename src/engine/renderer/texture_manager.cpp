#include "texture_manager.hpp"

#include<memory>

#include<GL/glew.h>


TextureManager::TextureManager()
{
    current_texture_id = -1;
}

TextureManager::~TextureManager(){}

void TextureManager::load_texture(const std::string& filename)
{
    std::unique_ptr<Texture> t(new Texture);
    t->load(filename);
    textures.push_back(std::move(t));
}

void TextureManager::start_texture(int id)
{
    current_texture_id = id;
    textures[id]->start();
}

void TextureManager::stop_texture()
{
    current_texture_id = -1;
    glBindTexture(GL_TEXTURE_2D, 0);
}

int TextureManager::get_active_texture()
{
    return current_texture_id;
}

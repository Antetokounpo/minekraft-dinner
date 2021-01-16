#pragma once

#include<string>
#include<vector>
#include<memory>

#include<engine/texture.hpp>

class TextureManager
{
    public:
        TextureManager();
        ~TextureManager();

        void load_texture(const std::string& filename);
        void start_texture(int id);
        void stop_texture();
        int get_active_texture();
    private:
        int current_texture_id;
        std::vector<std::unique_ptr<Texture>> textures;
};

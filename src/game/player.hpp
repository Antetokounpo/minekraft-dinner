#pragma once

#include<engine/renderer/camera.hpp>
#include<terrain/terrain.hpp>
#include<engine/model.hpp>
#include<engine/texture.hpp>

class Player : public Camera
{
    public:
        Player(SDL_Window* win);
        ~Player();

        void update(Terrain& t);
        void update_position(Terrain& t, float delta);

        bool check_collision(Terrain& t);
        void check_block_interaction(Terrain& t);

        const glm::vec3& get_player_position() const;

        void set_punching(bool b);
        void set_building(bool b);

    private:
        bool is_punching;
        bool is_building;

        glm::vec3 player_position;
        glm::vec3 hitbox;
};

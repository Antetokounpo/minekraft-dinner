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
        glm::vec3 get_ray();
        bool is_looking_at_face() const;
        Face get_looking_face() const;
        glm::vec3 get_looking_block() const;

        const glm::vec3& get_player_position() const;

        void set_punching(bool b);
        void set_building(bool b);

    private:
        bool is_punching;
        bool is_building;

        glm::vec3 player_position;
        bool is_looking_face;
        Face looking_face;
        glm::vec3 looking_block;
        float ray_range;
};

#pragma once

#include<engine/renderer/camera.hpp>
#include<terrain/terrain.hpp>
#include<engine/model.hpp>
#include<engine/texture.hpp>

class Player : public Camera, public Model, public Texture
{
    public:
        Player(SDL_Window* win);
        ~Player();

        void update(Terrain& t);
        void update_position(Terrain& t, float delta);

        bool check_collision(Terrain& t);

        const glm::vec3& get_player_position() const;

    private:
        glm::vec3 player_position;
        glm::vec3 hitbox;
        glm::vec3 acceleration;
};

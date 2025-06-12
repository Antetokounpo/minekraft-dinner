#pragma once

#include<vector>

#include<engine/skybox.hpp>
#include<terrain/terrain.hpp>
#include<engine/light.hpp>
#include "player.hpp"

class World
{
    public:
        World(SDL_Window* win);
        ~World();

        void update();
        void handle_events(SDL_Event e);
        Player& get_player();
        Terrain& get_terrain();
        Skybox& get_skybox();
        std::vector<Light>& get_light_sources();
    private:
        Terrain terrain;
        Skybox skybox;
        Player player;
        std::vector<Light> light_sources;
};

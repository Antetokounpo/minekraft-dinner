#pragma once

#include<vector>

#include<engine/skybox.hpp>
#include<terrain/terrain.hpp>
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
    private:
        Terrain terrain;
        Skybox skybox;
        Player player;
};

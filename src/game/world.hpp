#pragma once

#include<vector>

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
    private:
        Terrain terrain;
        Player player;
};

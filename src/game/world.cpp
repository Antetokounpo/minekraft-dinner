#include "world.hpp"

World::World(SDL_Window* win) : player(win) {}

World::~World() {}

void World::update()
{
    player.update(terrain);
}

void World::handle_events(SDL_Event e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
            player.set_punching(true);
        else if(e.button.button == SDL_BUTTON_RIGHT)
            player.set_building(true);
    }
}

Player& World::get_player()
{
    return player;
}

Terrain& World::get_terrain()
{
    return terrain;
}

Skybox& World::get_skybox()
{
    return skybox;
}

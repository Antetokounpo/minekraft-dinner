#include "world.hpp"

World::World(SDL_Window* win) : player(win) {}

World::~World() {}

void World::update()
{
    player.update(terrain);
}

Player& World::get_player()
{
    return player;
}

Terrain& World::get_terrain()
{
    return terrain;
}

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
    } else if(e.type == SDL_KEYDOWN)
    {
        int offset = 0;
        switch(e.key.keysym.scancode)
        {
            case SDL_SCANCODE_Q:
                offset = -1;
                break;
            case SDL_SCANCODE_E:
                offset = 1;
                break;
            default:
                break;
        }

        if(offset)
        {
            int new_building_block = static_cast<int>(player.get_building_block()) + offset;
            new_building_block = ((new_building_block % N_BLOCK) + N_BLOCK) % N_BLOCK;
            player.set_building_block(new_building_block);
        }
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

std::vector<Light> &World::get_light_sources()
{
    return light_sources;
}

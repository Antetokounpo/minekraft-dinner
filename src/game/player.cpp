#include "player.hpp"

#include<engine/renderer/camera.hpp>
#include<engine/loaders/objloader.hpp>

#include<iostream> // debug

Player::Player(SDL_Window* win) : Camera(win)
{
    hitbox = {1.0f, 1.0f, 1.0f};
    player_position = {0.0f, 20.0f, 0.0f};

    is_punching = false;

    Model::load("res/models/player.obj");
    Texture::load("res/tex/blue.png");
}

Player::~Player(){}

void Player::update_position(Terrain& t, float delta)
{
    float movement = delta * speed;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W])
        player_position += walking_direction * movement;
    if(state[SDL_SCANCODE_S])
        player_position -= walking_direction * movement;
    if(state[SDL_SCANCODE_D])
        player_position += right * movement;
    if(state[SDL_SCANCODE_A])
        player_position -= right * movement;
    if(state[SDL_SCANCODE_SPACE])
        player_position += up * movement;
    if(state[SDL_SCANCODE_LCTRL])
        player_position -= up * movement;

    position = player_position;//{player_position.x+0.5f, player_position.y+2, player_position.z+0.5f}; // camera position
}

void Player::update(Terrain& t)
{
    if(!(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS))
        return;

    float delta_time = get_time_delta();

    check_inputs();

    check_block_interaction(t);

    update_angles(delta_time);
    rotate();
    update_position(t, delta_time);

    reset_mouse();
    is_punching = mouse_state & SDL_BUTTON_LMASK; // On continue de puncher ssi le bouton est toujours enfoncé
}

bool Player::check_collision(Terrain& t)
{
    unsigned block;
    block = t.get_block(player_position.x, player_position.y, player_position.z);
    if(!block)
    {
        return false;
    }

    return true; 
}

void Player::check_block_interaction(Terrain& t)
{
    glm::vec3 looking_block = direction*2.0f + position;
    unsigned block = t.get_block(looking_block);

    if(block && is_punching)
        t.set_block(looking_block, 0);
}

const glm::vec3& Player::get_player_position() const
{
    return player_position;
}

void Player::set_punching(bool b)
{
    is_punching = b;
}

#include "player.hpp"

#include<engine/renderer/camera.hpp>
#include<engine/loaders/objloader.hpp>

#include<iostream> // debug

Player::Player(SDL_Window* win) : Camera(win)
{
    acceleration = glm::vec3(0.0f);
    hitbox = {1.0f, 1.0f, 1.0f};
    player_position = {0.0f, 20.0f, 0.0f};

    Model::load("res/models/player.obj");
    Texture::load("res/tex/blue.png");
}

Player::~Player(){}

void Player::update_position(Terrain& t, float delta)
{
    float movement = delta * speed;

    glm::vec3 old_position = player_position;

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

    if(check_collision(t))
        player_position = old_position;

    position = {player_position.x, player_position.y+2*hitbox.y, player_position.z-5*hitbox.z};
}

void Player::update(Terrain& t)
{
    if(!(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS))
        return;

    float delta_time = get_time_delta();

    check_inputs();

    update_angles(delta_time);
    rotate();
    update_position(t, delta_time);

    reset_mouse();
}

bool Player::check_collision(Terrain& t)
{
    /*
    Les collisions horizontales et verticales sont separées. Peu importe
    la face du joueur entre en collision seulement le block en question est
    nécessaire pour calculer le vecteur normal ou le vecteur collision. La hauteur
    dans le cas d'une collision horizontale n'est pas importante et il en va
    de même pour la collision verticale.
    */

    glm::vec3 minbox = player_position;
    glm::vec3 maxbox = minbox+hitbox;

    for(int i = -1; i<=1; ++i)
    for(int j = -1; j<=1; ++j)
    for(int k = -1; k<=1; ++k)
    {
        glm::vec3 d = {i, j, k};
        glm::vec3 minblock = player_position+d;
        glm::vec3 maxblock = minblock+glm::vec3(1.0f);
        if(!t.get_block(minblock))
            continue;
        else
        {
            bool c = (minbox.x <= maxblock.x && maxbox.x >= minblock.x) &&
                     (minbox.y <= maxblock.y && maxbox.y >= minblock.y) &&
                     (minbox.z <= maxblock.z && maxbox.z >= minblock.z);
            if(c) return true;
        }
    }

    return false;
}

const glm::vec3& Player::get_player_position() const
{
    return player_position;
}

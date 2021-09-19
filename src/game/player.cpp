#include "player.hpp"

#include<engine/renderer/camera.hpp>
#include<engine/loaders/objloader.hpp>

#include<iostream> // debug

Player::Player(SDL_Window* win) : Camera(win)
{
    player_position = {0.0f, 20.0f, 0.0f};

    is_punching = false;
    is_building = false;

    is_looking_face = false;
    looking_face = {0, 0, 0, SUD, 0};
    ray_range = 2.0f;
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
    is_building = mouse_state & SDL_BUTTON_RMASK;
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
    unsigned block;
    glm::vec3 ray = get_ray();
    unsigned increment_size = 80;
    float ray_increment = ray_range/increment_size;
    for(unsigned i = 0; i<increment_size; ++i)
    {
        looking_block = ray*(i*ray_increment) + position;
        block = t.get_block(looking_block);
        if(block)
            break;
    }

    glm::vec3 looking_block_rel = looking_block - glm::floor(looking_block);
    double delta_x = (ray.x > 0) ? glm::abs(looking_block_rel.x) : (1.0f - glm::abs(looking_block_rel.x));
    double delta_y = (ray.y > 0) ? glm::abs(looking_block_rel.y) : (1.0f - glm::abs(looking_block_rel.y));
    double delta_z = (ray.z > 0) ? glm::abs(looking_block_rel.z) : (1.0f - glm::abs(looking_block_rel.z));

    FaceOrientation face;
    if(glm::min(delta_x, delta_y) == delta_x && glm::min(delta_x, delta_z) == delta_x)
        face = (ray.x > 0) ? OUEST : EST;
    else if(glm::min(delta_y, delta_z) == delta_y)
        face = (ray.y > 0) ? DESSOUS : DESSUS;
    else
        face = (ray.z > 0) ? SUD : NORD;

    is_looking_face = (bool)block;
    looking_face = {(unsigned)floor(looking_block.x) % 16, (unsigned)floor(looking_block.y), (unsigned)floor(looking_block.z) % 16, face, 2};

    if(block && is_punching)
        t.set_block(looking_block, 0);
}

glm::vec3 Player::get_ray()
{
    glm::vec4 ray_clip = {0.0f, 0.0f, -1.0f, 1.0f};
    glm::vec4 ray_eye = glm::inverse(get_projection_matrix()) * ray_clip;
    ray_eye = {ray_eye.x, ray_eye.y, -1.0, 0.0};
    glm::vec4 ray_wor = glm::inverse(get_view_matrix()) * ray_eye;

    return glm::normalize(glm::vec3({ray_wor.x, ray_wor.y, ray_wor.z}));
}

bool Player::is_looking_at_face() const
{
    return is_looking_face;
}

Face Player::get_looking_face() const
{
    return looking_face;
}

glm::vec3 Player::get_looking_block() const
{
    return looking_block;
}

const glm::vec3& Player::get_player_position() const
{
    return player_position;
}

void Player::set_punching(bool b)
{
    is_punching = b;
}

void Player::set_building(bool b)
{
    is_building = b;
}
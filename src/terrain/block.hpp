#pragma once

#include<string>

typedef struct Block
{
    const std::string name;
    const unsigned block_id;
    const unsigned tex_face_s; // SUD
    const unsigned tex_face_e; // EST
    const unsigned tex_face_n; // NORD
    const unsigned tex_face_o; // OUEST
    const unsigned tex_face_u; // DESSUS
    const unsigned tex_face_d; // DESSOUS
    const bool transparent;
} Block;


static Block BLOCK_TYPES[] = {
    {"Air", 0}, // On se crisse du reste
    {"Dirt", 1, 0, 0, 0, 0, 0, 0, false},
    {"Stone", 2, 1, 1, 1, 1, 1, 1, false},
    {"Grass", 3, 3, 3, 3, 3, 2, 0, false},
    {"Sand", 4, 4, 4, 4, 4, 4, 0, false},
    {"Water", 5, 10, 10, 10, 10, 10, 10, true}
};

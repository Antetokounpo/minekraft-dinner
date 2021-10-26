#pragma once

#include<string>

typedef struct Block
{
    std::string name;
    unsigned block_id;
    unsigned tex_face_s; // SUD
    unsigned tex_face_e; // EST
    unsigned tex_face_n; // NORD
    unsigned tex_face_o; // OUEST
    unsigned tex_face_u; // DESSUS
    unsigned tex_face_d; // DESSOUS
    bool transparent;
} Block;


static Block BLOCK_TYPES[] = {
    {"Air", 0}, // On se crisse du reste
    {"Dirt", 1, 0, 0, 0, 0, 0, 0},
    {"Stone", 2, 1, 1, 1, 1, 1, 1},
    {"Grass", 3, 3, 3, 3, 3, 2, 0}
};

#pragma once

#include<string>

typedef struct Block
{
    std::string name;
    std::string texture_path;
} Block;


static Block BLOCK_TYPES[] = {
    {"Dirt", "res/tex/dirt_sq.png"},
    {"Stone", "res/tex/stone.png"}
};
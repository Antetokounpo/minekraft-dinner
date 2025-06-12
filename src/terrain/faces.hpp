#pragma once

#include<vector>
#include<array>
#include<glm/glm.hpp>

enum FaceOrientation
{
    SUD     ,
    EST     ,
    NORD    ,
    OUEST   ,
    DESSUS  ,
    DESSOUS ,
};

struct Face
{
    unsigned i;
    unsigned j;
    unsigned k;

    FaceOrientation face;
    unsigned texture_id;
};

static const unsigned atlas_w = 160;
static const unsigned atlas_h = 160;
static const unsigned tex_w = 16;
static const unsigned tex_h = 16;

static const std::array<glm::vec3, 6> faces_directions = 
{
    glm::vec3( 0.0,  0.0, -1.0), // SUD
    glm::vec3( 1.0,  0.0,  0.0), // EST
    glm::vec3( 0.0,  0.0,  1.0), // NORD
    glm::vec3(-1.0,  0.0,  0.0), // OUEST
    glm::vec3( 0.0,  1.0,  0.0), // DESSUS
    glm::vec3( 0.0, -1.0,  0.0), // DESSOUS
};

static const std::vector<std::array<float, 12>> faces =
{
    { // SUD
        0.0f,  1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    },
    { // EST
         1.0f,  1.0f, 0.0f,
         1.0f, 0.0f,  1.0f,
         1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,
    },
    { // NORD
         1.0f,  1.0f,  1.0f,
         0.0f, 0.0f,  1.0f,
         1.0f, 0.0f,  1.0f,
         0.0f,  1.0f,  1.0f,
    },
    { // OUEST
        0.0f,  1.0f,  1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,  1.0f,
        0.0f,  1.0f, 0.0f,
    },
    { // DESSUS
        0.0f, 1.0f,  1.0f,
         1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,  1.0f
    },
    { // DESSOUS
        0.0f, 0.0f, 0.0f,
         1.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
         1.0f, 0.0f, 0.0f
    },
};

static const std::vector<std::array<float, 12>> faces_normals =
{

};

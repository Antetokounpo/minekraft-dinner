#pragma once

#include<vector>

#include<glm/vec2.hpp>

class Perlin
{
    public:
        Perlin();
        ~Perlin();

        void set_seed(unsigned int seed);
        double perlin(double x, double y, double z=0.0f); // 3D
    private:
        double fade(double t);
        double lerp(double x, double a, double b);
        double grad(int hash, double x, double y, double z);
        double perlin_noise(double x, double y, double z);

        std::vector<unsigned> p;
};

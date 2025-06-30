#pragma once

#include<vector>

#include<glm/vec2.hpp>

class Perlin
{
    public:
        Perlin();
        ~Perlin();

        void set_seed(unsigned int seed);
        double perlin(double x, double y, double z=0.0f) const; // 3D
    private:
        double fade(double t) const;
        double lerp(double x, double a, double b) const;
        double grad(int hash, double x, double y, double z) const;
        double perlin_noise(double x, double y, double z) const;

        std::vector<unsigned> p;
};

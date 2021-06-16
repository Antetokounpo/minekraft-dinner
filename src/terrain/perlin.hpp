#pragma once

#include<vector>

class Perlin
{
    public:
        Perlin();
        ~Perlin();

        void set_seed(unsigned int seed);
        double perlin(double x, double y);
    private:
        double fade(double t);
        double grad(int hash, double x, double y);
        double lerp (double a, double b, double x);

        std::vector<unsigned int> p;
        int repeat;
};

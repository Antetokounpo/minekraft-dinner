#pragma once

#include "perlin.hpp"

class NoiseGenerator : public Perlin
{
    public:
        NoiseGenerator();
        ~NoiseGenerator();

        void set_frequency(double f);
        void set_amplitude(double a);

        double noise(double x, double y, double z=0.0f);
    private:
        double frequency;
        double amplitude;

};

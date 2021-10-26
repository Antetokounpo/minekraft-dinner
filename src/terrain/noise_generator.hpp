#pragma once

#include "perlin.hpp"

class NoiseGenerator : public Perlin
{
    public:
        NoiseGenerator();
        ~NoiseGenerator();

        void add_octave(double a, double f);

        double noise(double x, double y, double z=0.0f);
    private:
        std::vector<double> amplitudes;
        std::vector<double> frequencies;

        double frequency;
        double amplitude;

};

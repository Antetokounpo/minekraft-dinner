#include "noise_generator.hpp"

NoiseGenerator::NoiseGenerator()
{
    frequency = 1.0f;
    amplitude = 1.0f;
}

NoiseGenerator::~NoiseGenerator() {}

void NoiseGenerator::set_frequency(double f)
{
    frequency = f;
}

void NoiseGenerator::set_amplitude(double a)
{
    amplitude = a;
}

double NoiseGenerator::noise(double x, double y, double z)
{
    return amplitude * perlin(frequency*x, frequency*y, frequency*z);
}

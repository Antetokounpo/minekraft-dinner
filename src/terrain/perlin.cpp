#include "perlin.hpp"

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

Perlin::Perlin() {}

Perlin::~Perlin() {}

void Perlin::set_seed(unsigned int seed)
{
    std::vector<unsigned int> permutation;
    for (int i = 0; i < 256; ++i)
        permutation.push_back(i);

    std::srand(seed);
    std::random_shuffle(permutation.begin(), permutation.end());

    p.clear();
    for (int i = 0; i < 512; ++i)
        p.emplace_back(permutation[i % 256]);
}

double Perlin::fade(double t)
{
    return 6*std::pow(t, 5) - 15*std::pow(t, 4) + 10*std::pow(t, 3);
}

double Perlin::lerp(double x, double a, double b)
{
    return x*(b - a) + a;
}

double Perlin::grad(int hash, double x, double y, double z)
{
    int h = hash & 15;                      
    double u = h<8 ? x : y;
    double v = h<4 ? y : h==12||h==14 ? x : z;

    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double Perlin::perlin_noise(double x, double y, double z)
{
    int xi = (int)floor(x) & 255;
    int yi = (int)floor(y) & 255;
    int zi = (int)floor(z) & 255;

    double xf = x - floor(x);
    double yf = y - floor(y);                                
    double zf = z - floor(z);

    double u = fade(xf);
    double v = fade(yf);
    double w = fade(zf);

    int a = p[xi]+yi;
    int b = p[xi+1]+yi;

    int aa = p[a]+zi;
    int ab = p[a+1]+zi;
    int ba = p[b]+zi;
    int bb = p[b+1]+zi;

    return lerp(w, lerp(v, lerp(u, grad(p[aa  ], xf  , yf  , zf   ), 
                                   grad(p[ba  ], xf-1, yf  , zf   )),
                           lerp(u, grad(p[ab  ], xf  , yf-1, zf   ), 
                                   grad(p[bb  ], xf-1, yf-1, zf   ))),
                   lerp(v, lerp(u, grad(p[aa+1], xf  , yf  , zf-1 ), 
                                   grad(p[ba+1], xf-1, yf  , zf-1 )), 
                           lerp(u, grad(p[ab+1], xf  , yf-1, zf-1 ),
                                   grad(p[bb+1], xf-1, yf-1, zf-1 ))));
}

double Perlin::perlin(double x, double y, double z)
{
    double n = perlin_noise(x, y, z);

    /* Valeur de n entre -1 et 1 */ 
    return n * 0.5f + 0.5f;
}

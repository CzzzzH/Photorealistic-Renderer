#ifndef BRDF_HPP
#define BRDF_HPP

#include "util.hpp"

class BRDF
{
public:
    enum class Type { DIFFUSE, MIRROR, GLASS, LIGHT, FLOOR, WALL, CEIL, BUNNY, DRAGON};
    double specular, diffuse, refraction, refractiveIndex, rhoD, rhoS, phongS;
    Type type;
    BRDF() = delete;
    BRDF(double specular, double diffuse, double refraction,
         double rhoD, double rhoS, double phongS, double refractiveIndex, Type type)
    {
        this->specular = specular;
        this->diffuse = diffuse;
        this->refraction = refraction;
        this->rhoD = rhoD;
        this->rhoS = rhoS;
        this->phongS = phongS;
        this->refractiveIndex = refractiveIndex;
        this->type = type;
    }                                       
};

#endif // BRDF_HPP

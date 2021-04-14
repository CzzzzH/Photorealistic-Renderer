#ifndef VIEWPOINT_HPP
#define VIEWPOINT_HPP

#include "util.hpp"

class Material;

class ViewPoint
{
public:
    Point pos;
    Color weight, flux, fluxLight;
    Vector3f dir, normal;
    Material *material;
    double r2;
    int photonNum;

    bool valid;
    ViewPoint()
    {
        flux = fluxLight = Color(0);
        r2 = INIT_RADIUS;
        photonNum = 0;
        valid = false;
    }
};


#endif // VIEWPOINT_HPP

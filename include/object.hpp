#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "material.hpp"
#include "ray.hpp"
#include "hit.hpp"

using namespace std;

class Object
{
public:
    enum class ObjType {PLANE, TRIANGLE, SPHERE, DISC, BEZIER, MESH};
    Material *material;
    ObjType type;
    Point minPoint, maxPoint, centerPoint;
    Object(Material *m) : material(m) {}
    bool colorUsed = false;

    virtual ~Object() = default;
    virtual bool intersect(const Ray &r, Hit &h, double tmin) = 0;

};

#endif // OBJECT_HPP

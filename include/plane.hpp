#ifndef PLANE_H
#define PLANE_H

#include "object.hpp"

class Plane : public Object
{
public:

    Vector3f normal;
    double d;

    Plane() = delete;
    ~Plane() override = default;

    Plane(Vector3f normal, double d, Material *m) : Object(m)
    {
        this->normal = normal;
        this->d = d;
        this->type = ObjType::PLANE;
    }

    bool intersect(const Ray &ray, Hit &hit, double tmin) override
    {
        double t = (d - Vector3f::dot(normal, ray.getOrigin())) / (Vector3f::dot(normal, ray.getDirection()));
        if (t > hit.t || t < tmin) return false;
        if (Vector3f::dot(normal, ray.getDirection()) > 0) hit.set(t, this, normal * (-1));
        else hit.set(t, this, normal);
        return true;
    }
};

#endif //PLANE_H

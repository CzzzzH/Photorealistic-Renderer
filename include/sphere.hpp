#ifndef SPHERE_H
#define SPHERE_H

#include "object.hpp"

class Sphere : public Object
{
public:

    Point center;
    double radius;

    Sphere() = delete;
    Sphere(Point center, double radius, Material *m) : Object(m)
    {
        this->center = center;
        this->centerPoint = center;
        this->maxPoint = center + Vector3f(radius, radius, radius);
        this->minPoint = center - Vector3f(radius, radius, radius);
        this->radius = radius;
        this->type = ObjType::SPHERE;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &ray, Hit &hit, double tmin) override
    {
        Vector3f l = center - ray.getOrigin();
        if (l.length() <= radius) return false;
        double tp = Vector3f::dot(l, ray.getDirection());
        double d = l.length() * l.length() - tp * tp;
        if (radius * radius - d < EPS) return false;
        double t = tp - sqrt(radius * radius - d);
        if (t > hit.t || t < tmin) return false;
        Vector3f normal = ray.getOrigin() + t * ray.getDirection() - center;
        if (Vector3f::dot(normal, ray.getDirection()) > 0)
        {
            hit.set(t, this, normal * (-1));
            hit.inside = true;
        }
        else hit.set(t, this, normal);
        return true;
    }

};

#endif

#ifndef DISC_HPP
#define DISC_HPP

#include "plane.hpp"

class Disc : public Plane
{
public:

    Point center;
    double radius;

    Disc() = delete;
    Disc(Vector3f nm, double d, Point center, double radius, Material *m) : Plane(nm, d, m)
    {
        this->center = center;
        this->centerPoint = center;
        this->normal = nm.normalized();
        this->minPoint.x() = center.x() - radius * Vector3f::cross(Vector3f(1, 0, 0), this->normal).length();
        this->minPoint.y() = center.y() - radius * Vector3f::cross(Vector3f(0, 1, 0), this->normal).length();
        this->minPoint.z() = center.z() - radius * Vector3f::cross(Vector3f(0, 0, 1), this->normal).length();
        this->maxPoint.x() = center.x() + radius * Vector3f::cross(Vector3f(1, 0, 0), this->normal).length();
        this->maxPoint.y() = center.y() + radius * Vector3f::cross(Vector3f(0, 1, 0), this->normal).length();
        this->maxPoint.z() = center.z() + radius * Vector3f::cross(Vector3f(0, 0, 1), this->normal).length();
        this->radius = radius;
        this->type = ObjType::DISC;
    }

    ~Disc() override = default;

    bool intersect(const Ray &ray, Hit &hit, double tmin) override
    {
        Hit testHit = hit;
        if (!Plane::intersect(ray, testHit, tmin)) return false;
        Point hitPoint = ray.pointAtParameter(testHit.t);
        if ((hitPoint - center).length() > radius) return false;
        hit.set(testHit.t, this, testHit.normal);
        return true;
    }
};

#endif // DISC_HPP

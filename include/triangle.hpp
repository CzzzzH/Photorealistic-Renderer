#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.hpp"

using namespace std;

class Mesh;

class Triangle : public Object
{
public:
    Vector3f normal;
    Point vertices[3];
    Color verticesColor[3];
    Mesh *parent;
    double d;

	Triangle() = delete;

    Triangle(Point a, Point b, Point c, Material *m, Mesh *p) : Object(m), parent(p)
	{
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        normal = Vector3f::cross(b - a, c - a).normalized();
        d = Vector3f::dot(normal, a);
        this->centerPoint = (a + b + c) / 3;
        this->maxPoint = Vector3f::max(a, Vector3f::max(b, c));
        this->minPoint = Vector3f::min(a, Vector3f::min(b, c));
        this->type = ObjType::TRIANGLE;
	}

    bool intersect(const Ray &ray, Hit &hit, double tmin) override
    {
        double t = (d - Vector3f::dot(normal, ray.getOrigin())) / (Vector3f::dot(normal, ray.getDirection()));
        if (t > hit.t || t < tmin) return false;
        Point p = ray.pointAtParameter(t);
        if (Vector3f::dot(normal, Vector3f::cross(vertices[0] - p, vertices[1] - p)) < 0) return false;
        if (Vector3f::dot(normal, Vector3f::cross(vertices[1] - p, vertices[2] - p)) < 0) return false;
        if (Vector3f::dot(normal, Vector3f::cross(vertices[2] - p, vertices[0] - p)) < 0) return false;
        if (Vector3f::dot(normal, ray.getDirection()) > 0)
        {
            hit.set(t, this, normal * (-1));
            hit.inside = true;
        }
        else hit.set(t, this, normal);
        return true;
    }
};

#endif //TRIANGLE_H

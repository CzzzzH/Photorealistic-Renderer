#ifndef PARAMETRICSURFACE_HPP
#define PARAMETRICSURFACE_HPP

#include "quadtree.h"

class ParmetricSurface : public Object
{
public:
    enum class SurfaceType {TREFOIL, TORUS};
    Vector3f normal;
    Point now;
    double r;
    SurfaceType surfaceType;
    QuadTree quadTree;

    ParmetricSurface() = delete;
    ~ParmetricSurface() override = default;

    ParmetricSurface(Vector3f center, double radius, SurfaceType stype, Material *m) : Object(m)
    {
        this->centerPoint = center;
        this->r = radius;
        this->surfaceType = stype;

        switch (surfaceType)
        {
            case SurfaceType::TREFOIL:
            {
                maxPoint = Vector3f(1, 2.7358148, 1.611328125) * radius + center;
                minPoint = Vector3f(-1, -2.7358148, -1.611328125) * radius + center;
                break;
            }
            case SurfaceType::TORUS:
            {
                maxPoint = Vector3f(1.5, 0.5, 1.5) * radius + center;
                minPoint = Vector3f(-1.5, -0.5, -1.5) * radius + center;
                break;
            }
        }
        quadTree.face = this;
        quadTree.root = quadTree.build(1, 0, 2 * PI, 0, 2 * PI);
    }

    Point getPoint(double u, double v)
    {
        Point res;
        switch (surfaceType)
        {
            case SurfaceType::TREFOIL:
            {
                double temp = 2 * PI / 3;
                res.x() = r * sin(3 * u) / (2 + cos(v)) + centerPoint.x();
                res.y() = r * (sin(u) + 2 * sin(2 * u)) / (2 + cos(v + temp)) + centerPoint.y();
                res.z() = 0.125 * r * (cos(u) - 2 * cos(2 * u)) * (2 + cos(v)) * (2 + cos(v + temp)) + centerPoint.z();
                break;
            }
            case SurfaceType::TORUS:
            {
                res.x() = (r + 0.05 * cos(v)) * sin(u) + centerPoint.x();
                res.y() = 0.05 * sin(v) + centerPoint.y();
                res.z() = (r + 0.05 * cos(v)) * cos(u) + centerPoint.z();
                break;
            }
        }
        return res;
    }

    // 获得一系列参数
    void getParamter(const Ray &ray, double t, double u, double v, Point &J1, Point &J2, Point &J3, Vector3f &df, double &D)
    {
        Vector3f psu, psv;
        Point dir = ray.getDirection();
        switch (surfaceType)
        {
            case SurfaceType::TREFOIL:
            {
                double temp = PI / 6;
                psu.x() = (3 * r * cos(3 * u)) / (cos(v) + 2);
                psv.x() = (r * sin(3 * u) * sin(v)) / ((cos(v) + 2) * (cos(v) + 2));
                psu.y() = (r * (cos(u) + 4 * cos(2 * u))) / (2 - sin(v + temp));
                psv.y() = r *cos(v + temp) * (sin(u) + 2 * sin(2 * u)) / ((2 - sin(v + temp)) *(2 - sin(v + temp)));
                psu.z() = 0.125 * r * (sin(u) - 4 * sin(2 * u)) * (sin(v + temp) - 2) * (cos(v) + 2);
                psv.z() = 0.125 * r * (cos(u) - 2 * cos(2 * u)) * (sin(v) * (sin(v + temp) - 2) - (cos(v) + 2) * cos(v + temp));
                break;
            }
            case SurfaceType::TORUS:
            {
                psu.x() = cos(u) * (0.05 * cos(v) + r);
                psv.x() = - sin(u) * sin(v);
                psu.y() = 0;
                psv.y() = 0.05 * cos(v);
                psu.z() = - 0.05 * sin(u) * (0.05 * cos(v) + r);
                psv.z() = - 0.05 * cos(u) * sin(v);
            }
        }
        normal = Vector3f::cross(psu, psv);
        D = dir.x() * psu.y() * psv.z() + psu.x() * psv.y() + dir.z() + psv.x() * dir.y() * psu.z();
        D -= dir.z() * psu.y() * psv.x() + dir.x() * psu.z() * psv.y() + dir.y() * psu.x() * psv.z();
        J1.x() = psu.y() * psv.z() - psu.z() * psv.y();
        J1.y() = psu.z() * psv.x() - psu.x() * psv.z();
        J1.z() = psu.x() * psv.y() - psv.x() * psu.y();
        J2.x() = psv.y() * dir.z() - psv.z() * dir.y();
        J2.y() = psv.z() * dir.x() - psv.x() * dir.z();
        J2.z() = psv.x() * dir.y() - dir.x() * psv.y();
        J3.x() = dir.y() * psu.z() - dir.z() * psu.y();
        J3.y() = dir.z() * psu.x() - dir.x() * psu.z();
        J3.z() = dir.x() * psu.y() - psu.x() * dir.y();
        D = 1 / D;
        J1 = J1 * D;
        J2 = J2 * D;
        J3 = J3 * D;
        df = ray.pointAtParameter(t) - getPoint(u, v);
    }

    // 牛顿迭代求交
    bool intersect(const Ray &ray, Hit &hit, double tmin) override
    {
        return quadTree.intersect(quadTree.root, ray, hit, tmin);
    }
};

#endif // PARAMETRICSURFACE_HPP

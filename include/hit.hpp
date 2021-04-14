#ifndef HIT_H
#define HIT_H

#include "util.hpp"

class Object;

class Hit
{
public:
    double t;
    Object *hitObj;
    Vector3f normal;
    bool inside = false;

    // constructors
    Hit()
    {
        hitObj = nullptr;
        t = HINF;
    }

    Hit(double _t, Object *obj, const Vector3f &n)
    {
        t = _t;
        hitObj = obj;
        normal = n.normalized();
    }

    Hit(const Hit &h)
    {
        t = h.t;
        hitObj = h.hitObj;
        normal = h.normal.normalized();
    }

    // destructor
    ~Hit() = default;

    void set(double _t, Object *obj, const Vector3f &n)
    {
        t = _t;
        hitObj = obj;
        normal = n.normalized();
    }


};

#endif // HIT_H

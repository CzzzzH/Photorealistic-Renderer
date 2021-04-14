#ifndef MESH_H
#define MESH_H

#include "triangle.hpp"

using namespace std;

class Mesh : public Object
{

public:
    Mesh(Material *m) : Object(m)
    {
        this->type = ObjType::MESH;
        maxPoint = Vector3f(-INF);
        minPoint = Vector3f(INF);
    }
    vector<Triangle*> face;
    vector<Point> vertex;

    void addFace(Triangle *triangleFace)
    {
        face.push_back(triangleFace);
        triangleFace->parent = this;
    }
    void getMaxPoint()
    {
        for (int i = 0; i < vertex.size(); ++i)
            maxPoint = Vector3f::max(maxPoint, vertex[i]);
    }

    void getMinPoint()
    {
        for (int i = 0; i < vertex.size(); ++i)
            minPoint = Vector3f::min(minPoint, vertex[i]);
    }

    void getCenterPoint()
    {
        Vector3f sum = Vector3f::ZERO;
        for (int i = 0; i < vertex.size(); ++i)
            sum += vertex[i];
        centerPoint = sum / vertex.size();
    }

    bool intersect(const Ray &r, Hit &h, double tmin) override { return false; }
};

#endif

#ifndef OBJKDTREE_H
#define OBJKDTREE_H

#include "util.hpp"
#include "object.hpp"

using namespace std;

class ObjKDTreeNode
{
    public:
    Vector3f lowerBound, upperBound;
    vector<Object*> objList;
    ObjKDTreeNode *leftChild, *rightChild;
    static bool compareX(Object *a, Object *b) {return a->centerPoint.x() < b->centerPoint.x();}
    static bool compareY(Object *a, Object *b) {return a->centerPoint.y() < b->centerPoint.y();}
    static bool compareZ(Object *a, Object *b) {return a->centerPoint.z() < b->centerPoint.z();}
    bool inside(Object* target)
    {
        bool xMinInside = target->maxPoint.x() > lowerBound.x() - EPS;
        bool xMaxInside = target->minPoint.x() < upperBound.x() + EPS;
        bool yMinInside = target->maxPoint.y() > lowerBound.y() - EPS;
        bool yMaxInside = target->minPoint.y() < upperBound.y() + EPS;
        bool zMinInside = target->maxPoint.z() > lowerBound.z() - EPS;
        bool zMaxInside = target->minPoint.z() < upperBound.z() + EPS;
        return xMinInside && xMaxInside && yMinInside && yMaxInside && zMinInside && zMaxInside;
    }
};

class ObjKDTree
{
public:

    ObjKDTreeNode *root;

    ObjKDTreeNode* build(int depth, int axis, const vector<Object*> &obj, Point lb, Point ub);
    double getCubeDistance(ObjKDTreeNode* node, const Ray &ray);
    void intersect(ObjKDTreeNode* node, const Ray &ray, Hit &hit, double tmin);

};

#endif // OBJKDTREE_H

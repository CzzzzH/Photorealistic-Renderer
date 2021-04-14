#ifndef VPKDTREE_H
#define VPKDTREE_H

#include "util.hpp"
#include "viewpoint.hpp"

using namespace std;

class Camera;

class VpKDTreeNode
{
public:
    static bool compareX(ViewPoint *a, ViewPoint *b) {return a->pos.x() < b->pos.x();}
    static bool compareY(ViewPoint *a, ViewPoint *b) {return a->pos.y() < b->pos.y();}
    static bool compareZ(ViewPoint *a, ViewPoint *b) {return a->pos.z() < b->pos.z();}
    ViewPoint *viewPoint;
    Vector3f lowerBound, upperBound;
    double maxR2;
    VpKDTreeNode *leftChild, *rightChild;
};

class VpKDTree
{
public:
    vector<ViewPoint*> vpList;
    VpKDTreeNode* build(int l, int r, int axis);
    void destruct(VpKDTreeNode *nowNode);
    VpKDTreeNode *root;

    VpKDTree(Camera* camera);
    ~VpKDTree();
    void update(VpKDTreeNode *nowNode, const Point &photonPos, const Color &weight, const Vector3f &dir);
};

#endif // VPKDTREE_H

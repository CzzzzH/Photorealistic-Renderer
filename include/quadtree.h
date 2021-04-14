#ifndef QUADTREE_H
#define QUADTREE_H

#include "object.hpp"

class ParmetricSurface;

class QuadTreeNode
{
public:
    double minU, maxU, minV, maxV, centerU, centerV;
    bool leaf = false;
    Point center, lowerBound, upperBound;
    QuadTreeNode* child[4];

    double intersect(const Ray &ray)
    {
        Vector3f source = ray.getOrigin(), dir = ray.getDirection();
        double tEnter = -INF, tExit = INF;

        if (source <= upperBound && source >= lowerBound) return 0;
        if (fabs(dir.x()) > 0)
        {
            tEnter = max(tEnter, min((lowerBound.x() - source.x()) / dir.x(), (upperBound.x() - source.x()) / dir.x()));
            tExit = min(tExit, max((lowerBound.x() - source.x()) / dir.x(), (upperBound.x() - source.x()) / dir.x()));
        }
        if (fabs(dir.y()) > 0)
        {
            tEnter = max(tEnter, min((lowerBound.y() - source.y()) / dir.y(), (upperBound.y() - source.y()) / dir.y()));
            tExit = min(tExit, max((lowerBound.y() - source.y()) / dir.y(), (upperBound.y() - source.y()) / dir.y()));
        }
        if (fabs(dir.z()) > 0)
        {
            tEnter = max(tEnter, min((lowerBound.z() - source.z()) / dir.z(), (upperBound.z() - source.z()) / dir.z()));
            tExit = min(tExit, max((lowerBound.z() - source.z()) / dir.z(), (upperBound.z() - source.z()) / dir.z()));
        }
        if (tEnter < -EPS) return INF;
        Point hitPoint = ray.pointAtParameter(tEnter);
        if (!(hitPoint >= lowerBound - Vector3f(2e-3)
            && hitPoint <= upperBound + Vector3f(2e-3))) return INF;
        return (tEnter + tExit) / 2;
    }

    double getCubeDistance(const Ray &ray)
    {
        Vector3f source = ray.getOrigin(), dir = ray.getDirection();
        double t = -INF;
        Vector3f &lb = lowerBound, &ub = upperBound;
        if (source <= ub && source >= lb) return t;
        if (fabs(dir.x()) > 0)
            t = max(t, min((lb.x() - source.x()) / dir.x(), (ub.x() - source.x()) / dir.x()));
        if (fabs(dir.y()) > 0)
            t = max(t, min((lb.y() - source.y()) / dir.y(), (ub.y() - source.y()) / dir.y()));
        if (fabs(dir.z()) > 0)
            t = max(t, min((lb.z() - source.z()) / dir.z(), (ub.z() - source.z()) / dir.z()));
        if (t < -EPS) return INF;
        Point hitPoint = ray.pointAtParameter(t);
        if (!(hitPoint >= lowerBound - Vector3f(2e-3)
            && hitPoint <= upperBound + Vector3f(2e-3))) return INF;
        return t;
    }
};

class QuadTree
{
public:
    struct NextNode
    {
        double t;
        QuadTreeNode *node;
        NextNode(QuadTreeNode *now) : node(now){ t = INF; }
        bool operator< (const NextNode & b) { return t < b.t;}
    };

    ParmetricSurface* face;
    QuadTreeNode *root;
    QuadTreeNode* build(int depth, double minU, double maxU, double minV, double maxV);
    bool intersect(QuadTreeNode* nowNode, const Ray &ray, Hit &hit, double tMin);

};

#endif // QUADTREE_H

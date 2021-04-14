#include "parametricsurface.hpp"

QuadTreeNode *QuadTree::build(int depth, double minU, double maxU, double minV, double maxV)
{
    auto nowNode = new QuadTreeNode;
    nowNode->minU = minU;
    nowNode->maxU = maxU;
    nowNode->minV = minV;
    nowNode->maxV = maxV;
    nowNode->centerU = minU + (maxU - minU) / 2;
    nowNode->centerV = minV + (maxV - minV) / 2;
    nowNode->center = face->getPoint(nowNode->centerU, nowNode->centerV);
    nowNode->lowerBound = Vector3f(INF);
    nowNode->upperBound = Vector3f(-INF);

    if (depth < MAX_QUAD_TREE_DEPTH)
    {
        nowNode->leaf = false;
        nowNode->child[0] = build(depth + 1, minU, nowNode->centerU, minV, nowNode->centerV);
        nowNode->child[1] = build(depth + 1, nowNode->centerU, maxU, minV, nowNode->centerV);
        nowNode->child[2] = build(depth + 1, minU, nowNode->centerU, nowNode->centerV, maxV);
        nowNode->child[3] = build(depth + 1, nowNode->centerU, maxU, nowNode->centerV, maxV);
        for (int i = 0; i < 4; ++i)
        {
            nowNode->upperBound = Vector3f::max(nowNode->upperBound, nowNode->child[i]->upperBound);
            nowNode->lowerBound = Vector3f::min(nowNode->lowerBound, nowNode->child[i]->lowerBound);
        }
    }
    else
    {
        nowNode->leaf = true;
        for (int i = 0; i <= DISCRETE_NUM; ++i)
            for (int j = 0; j <= DISCRETE_NUM; ++j)
            {
                double u = minU + (maxU - minU) * i / DISCRETE_NUM;
                double v = minV + (maxV - minV) * i / DISCRETE_NUM;
                nowNode->upperBound = Vector3f::max(nowNode->upperBound, face->getPoint(u, v));
                nowNode->lowerBound = Vector3f::min(nowNode->lowerBound, face->getPoint(u, v));
            }
    }
    return nowNode;
}

bool QuadTree::intersect(QuadTreeNode *nowNode, const Ray &ray, Hit &hit, double tMin)
{
    bool flag = false;
    Vector3f source = ray.getOrigin(), dir = ray.getDirection();

    if (!nowNode->leaf)
    {
        NextNode next[4] = {NextNode(nowNode->child[0]), NextNode(nowNode->child[1]), NextNode(nowNode->child[2]), NextNode(nowNode->child[3])};
        for (int i = 0; i < 4; ++i)
            next[i].t = next[i].node->getCubeDistance(ray);
        sort(next, next + 4);
        for (int i = 0; i < 4; ++i)
        {
            if (next[i].t >= hit.t) break;
            if (intersect(next[i].node, ray, hit, tMin)) flag = true;
        }
        return flag;
    }
    else
    {
        double t, u, v, D;
        Point J1, J2, J3, df;
        t = nowNode->intersect(ray);
        if (t > hit.t) return false;
        u = nowNode->centerU;
        v = nowNode->centerV;
        face->getParamter(ray, t, u, v, J1, J2, J3, df, D);
        if (df.squaredLength() > NEWTON_DIS)
        {
            for (int i = 0; i < 6; ++i)
            {
                //cout << "[Iter " << i << "] Now Length: " << df.squaredLength() << endl;
                t -= Vector3f::dot(J1, df);
                u -= Vector3f::dot(J2, df);
                v -= Vector3f::dot(J3, df);
                face->getParamter(ray, t, u, v, J1, J2, J3, df, D);
                if (df.squaredLength() < NEWTON_DIS) break;
            }
        }
        if (t < tMin || t >= hit.t) return false;
        if (df.squaredLength() > NEWTON_DIS || u < 0 || v < 0 || u > 2 * PI || v > 2 * PI) return false;

        face->normal.normalize();
        if (Vector3f::dot(face->normal, dir) > 0)
        {
            hit.set(t, face, face->normal * (-1));
            hit.inside = true;
        }
        else hit.set(t, face, face->normal);
        return true;
    }
}

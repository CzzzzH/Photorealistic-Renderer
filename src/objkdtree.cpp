#include "objkdtree.h"

using namespace std;

// 递归生成加速物体求交的KD树
ObjKDTreeNode* ObjKDTree::build(int depth, int axis, const vector<Object*> &obj, Point lb, Point ub)
{
    ObjKDTreeNode *nowNode = new ObjKDTreeNode();
    nowNode->lowerBound = lb;
    nowNode->upperBound = ub;
    nowNode->leftChild = nowNode->rightChild = nullptr;
    for (int i = 0; i < obj.size(); ++i)
        if (nowNode->inside(obj[i])) nowNode->objList.push_back(obj[i]);
    if (depth >= MAX_OBJ_TREE_DEPTH || nowNode->objList.size() <= MAX_OBJ_NUM) return nowNode;
    axis %= 3;
    Point newUB, newLB;
    switch (axis)
    {
        case 0:
        {
            newUB = Vector3f((lb.x() + ub.x()) / 2, ub.y(), ub.z());
            newLB = Vector3f((lb.x() + ub.x()) / 2, lb.y(), lb.z());
            break;
        }
        case 1:
        {
            newUB = Vector3f(ub.x(), (lb.y() + ub.y()) / 2, ub.z());
            newLB = Vector3f(lb.x(), (lb.y() + ub.y()) / 2, lb.z());
            break;
        }
        case 2:
        {
            newUB = Vector3f(ub.x(), ub.y(), (lb.z() + ub.z()) / 2);
            newLB = Vector3f(lb.x(), lb.y(), (lb.z() + ub.z()) / 2);
            break;
        }
    }
    nowNode->leftChild = build(depth + 1, axis + 1, nowNode->objList, lb, newUB);
    nowNode->rightChild = build(depth + 1, axis + 1, nowNode->objList, newLB, ub);
    nowNode->objList.clear();
    map<Object*, int> count;
    for (int i = 0; i < nowNode->leftChild->objList.size(); ++i)
        count[nowNode->leftChild->objList[i]]++;
    for (int i = 0; i < nowNode->rightChild->objList.size(); ++i)
        count[nowNode->rightChild->objList[i]] += 2;
    nowNode->leftChild->objList.clear();
    nowNode->rightChild->objList.clear();
    for (auto it = count.begin(); it != count.end(); ++it)
    {
        if (it->second == 1) nowNode->leftChild->objList.push_back(it->first);
        else if (it->second == 2) nowNode->rightChild->objList.push_back(it->first);
        else nowNode->objList.push_back(it->first);
    }
    return nowNode;
}

// 返回到KD树节点的最短距离
double ObjKDTree::getCubeDistance(ObjKDTreeNode* node, const Ray &ray)
{
    double t = -INF;
    Vector3f source = ray.getOrigin(), dir = ray.getDirection();
    if (source <= node->upperBound && source >= node->lowerBound) return -INF;
    Vector3f &lb = node->lowerBound, &ub = node->upperBound;
    if (fabs(dir.x()) > 0)
        t = max(t, min((lb.x() - source.x()) / dir.x(), (ub.x() - source.x()) / dir.x()));
    if (fabs(dir.y()) > 0)
        t = max(t, min((lb.y() - source.y()) / dir.y(), (ub.y() - source.y()) / dir.y()));
    if (fabs(dir.z()) > 0)
        t = max(t, min((lb.z() - source.z()) / dir.z(), (ub.z() - source.z()) / dir.z()));
    if (t < -EPS) return INF;
    Point hitPoint = ray.pointAtParameter(t);
    if (!(hitPoint >= node->lowerBound && hitPoint <= node->upperBound)) return INF;
    return t;
}

// 求交
void ObjKDTree::intersect(ObjKDTreeNode* node, const Ray &ray, Hit &hit, double tmin)
{
    for (int i = 0; i < node->objList.size(); ++i)
        node->objList[i]->intersect(ray, hit, tmin);

    double tl = INF, tr = INF;
    if (node->leftChild == nullptr && node->rightChild == nullptr) return;
    if (node->leftChild) tl = getCubeDistance(node->leftChild, ray);
    if (node->rightChild) tr = getCubeDistance(node->rightChild, ray);

    // 当前最近交点比左右字节点的最短距离还小，可以剪枝
    if (hit.t <= min(tl, tr)) return;

    // 优先和距离近的节点求交（这样剪枝的效率更高）
    if (tl < tr)
    {
        intersect(node->leftChild, ray, hit, tmin);
        if (hit.t > tr) intersect(node->rightChild, ray, hit, tmin);
    }
    else
    {
        intersect(node->rightChild, ray, hit, tmin);
        if (hit.t > tl) intersect(node->leftChild, ray, hit, tmin);
    }

    return;
}

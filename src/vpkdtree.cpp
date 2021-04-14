#include "vpkdtree.h"
#include "camera.h"
#include "material.hpp"

using namespace std;

/* 目前用KD树的形式存储ViewPoint，看情况之后可能改成Hash Grid */

// 构造ViewPoint的KD树
VpKDTree::VpKDTree(Camera *camera)
{
    vpList.resize(camera->viewPoint.size());
    for (int i = 0; i < camera->viewPoint.size(); ++i)
    {
        vpList[i] = &(camera->viewPoint[i]);
        //cout << "[ViewPoint " << i << "] Pos: ";
        //vpList[i]->pos.print();
    }
    root = build(0, vpList.size() - 1, 0);
}

// 析构ViewPoint的KD树
VpKDTree::~VpKDTree()
{
    if (root != nullptr)
        destruct(root);
}

void VpKDTree::destruct(VpKDTreeNode *nowNode)
{
    if (nowNode->leftChild) destruct(nowNode->leftChild);
    if (nowNode->rightChild) destruct(nowNode->rightChild);
    delete nowNode;
}

// 递归构建KD树
VpKDTreeNode* VpKDTree::build(int l, int r, int axis)
{
    VpKDTreeNode *nowNode = new VpKDTreeNode();
    nowNode->lowerBound = Vector3f(INF);
    nowNode->upperBound = Vector3f(-INF);
    nowNode->maxR2 = 0;
    nowNode->leftChild = nowNode->rightChild = nullptr;
    // 确定节点边界
    for (int i = l; i <= r; ++i)
    {
        nowNode->lowerBound = Vector3f::min(nowNode->lowerBound, vpList[i]->pos);
        nowNode->upperBound = Vector3f::max(nowNode->upperBound, vpList[i]->pos);
        nowNode->maxR2 = max(nowNode->maxR2, vpList[i]->r2);
    }
    int m = (l + r) >> 1;
    axis %= 3;
    // 利用nth_element得到中位数确定分割线
    switch (axis)
    {
        case 0: nth_element(vpList.begin() + l, vpList.begin() + m, vpList.begin() + r + 1, VpKDTreeNode::compareX); break;
        case 1: nth_element(vpList.begin() + l, vpList.begin() + m, vpList.begin() + r + 1, VpKDTreeNode::compareY); break;
        case 2: nth_element(vpList.begin() + l, vpList.begin() + m, vpList.begin() + r + 1, VpKDTreeNode::compareZ); break;
    }
    nowNode->viewPoint = vpList[m];
    if (l < m) nowNode->leftChild = build(l, m - 1, axis + 1);
    if (r > m) nowNode->rightChild = build(m + 1, r, axis + 1);
    return nowNode;
}

// 更新ViewPoint的光通量
void VpKDTree::update(VpKDTreeNode *nowNode, const Point &photonPos, const Color &weight, const Vector3f &dir)
{
    // 如果和KD树节点的边界距离已经超过最大收光半径，就直接剪枝
    if (nowNode == nullptr) return;
    double minDis = 0;
    if (photonPos.x() > nowNode->upperBound.x()) minDis += pow(photonPos.x() - nowNode->upperBound.x(), 2);
    if (photonPos.x() < nowNode->lowerBound.x()) minDis += pow(photonPos.x() - nowNode->lowerBound.x(), 2);
    if (photonPos.y() > nowNode->upperBound.y()) minDis += pow(photonPos.y() - nowNode->upperBound.y(), 2);
    if (photonPos.y() < nowNode->lowerBound.y()) minDis += pow(photonPos.y() - nowNode->lowerBound.y(), 2);
    if (photonPos.z() > nowNode->upperBound.z()) minDis += pow(photonPos.z() - nowNode->upperBound.z(), 2);
    if (photonPos.z() < nowNode->lowerBound.z()) minDis += pow(photonPos.z() - nowNode->lowerBound.z(), 2);
    if (minDis > nowNode->maxR2) return;
    /*
    if ((photonPos - nowNode->viewPoint->pos).squaredLength() < 0.001)
    {
        cout << "[Check Update]" << endl;
        cout << "Photon Pos: ";
        photonPos.print();
        cout << "ViewPoint Pos: ";
        nowNode->viewPoint->pos.print();
        cout << "Dis2: " << (photonPos - nowNode->viewPoint->pos).squaredLength() << endl;
    }
    */

    // 如果当前节点存储的 ViewPoint 更新光通量 flux
    if (nowNode->viewPoint->valid && (photonPos - nowNode->viewPoint->pos).squaredLength() <= nowNode->viewPoint->r2)
    {
        auto vp = nowNode->viewPoint;
        // 计算衰减因子
        double factor = (vp->photonNum * ALPHA + ALPHA) / (vp->photonNum * ALPHA + 1.0);
        vp->r2 *= factor;
        Vector3f specularDir = dir - vp->normal * (2 * Vector3f::dot(dir, vp->normal));
        double rho = vp->material->brdf.rhoD + vp->material->brdf.rhoS * pow(Vector3f::dot(specularDir ,vp->dir), vp->material->brdf.phongS);
        rho = max(min(rho, 1.0), 0.0);
        vp->flux = (vp->flux + vp->weight * weight * rho) * factor;
        vp->photonNum++;
        /*
        cout << "[Update ViewPoint]" << endl << "Flux: ";
        vp->flux.print();
        cout<< "Weight: ";
        vp->weight.print();
        cout << "R2: " << vp->r2 << " Factor: " << factor << " Num: " << vp->photonNum << endl;
        */
    }

    // 递归向下更新节点
    if (nowNode->leftChild) update(nowNode->leftChild, photonPos, weight, dir);
    if (nowNode->rightChild) update(nowNode->rightChild, photonPos, weight, dir);
    nowNode->maxR2 = nowNode->viewPoint->r2;
    if (nowNode->leftChild) nowNode->maxR2 = max(nowNode->maxR2, nowNode->leftChild->maxR2);
    if (nowNode->rightChild) nowNode->maxR2 = max(nowNode->maxR2, nowNode->rightChild->maxR2);
}



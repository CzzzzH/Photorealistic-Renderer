#ifndef SCENE_H
#define SCENE_H

#include "disc.hpp"
#include "triangle.hpp"
#include "sphere.hpp"
#include "mesh.hpp"
#include "parametricsurface.hpp"
#include "objkdtree.h"
#include "vpkdtree.h"

using namespace std;

class Scene
{
public:

    vector<Object*> objList;
    vector<Material> materials;
    ObjKDTree objTree;
    VpKDTree *vpTree = nullptr;

    Point lightSource;
    Vector3f lightNormal;
    double lightRadius;
    int traceCount = 0;

    Scene(int sceneNum);
    ~Scene();
    void addPlane();
    void addTriangle(Point a, Point b, Point c, Material *m);
    void addSphere(Point center, double radius, Material *m);
    void addParametricSurface(Vector3f offset, double radius, ParmetricSurface::SurfaceType tp, Material *m);
    void addPlyMesh(string filePath, Material *m, double centerX, double centerZ, double size, double rotateXZ, double height, string uvTexturePath = "");
    void addLight(Vector3f normal, double d, Point center, double radius, Material *m);
    void addWall(int sceneNum);
    void buildObjTree();

    Ray generateRay(ll iter);
    Vector3f randomReflectedRay(Vector3f normal, int depth, double shiniess, ll iter);
    Vector3f randomLightReflectedRay(Vector3f normal, int depth, double shiniess, ll iter);
    void trace(const Ray &ray, const Color &weight, int depth, ll iter, ViewPoint *vp);

};

#endif // SCENE_H

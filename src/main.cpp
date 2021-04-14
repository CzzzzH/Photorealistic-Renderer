#include <iostream>
#include "camera.h"
#include "scene.h"

using namespace std;

void renderDragonScene()
{
    int sceneNum = 0;
    Scene *scene = new Scene(sceneNum);
    Camera *camera = new Camera(0.684, 0.811, EPS, 1.09, 4096, 3072, Point(0.05, 0.15, -0.45), scene);
    scene->addWall(sceneNum);
    scene->addSphere(Point(-0.15, 0.3, 0.15), 0.07, &scene->materials[3]);
    scene->addSphere(Point(0.15, 0.6, 0.3), 0.07, &scene->materials[3]);


    scene->addPlyMesh("./material/bunny.ply",
                      &scene->materials[2], 0.25, 0.1, 2.5, 0, 0, "");

    scene->addPlyMesh("./material/dragonT.ply",
                      &scene->materials[4], -0.3, 0.2, 0.005, - PI / 6, 0.3, "./material/dragon_blue.bmp");


    scene->addPlyMesh("./material/dragonT.ply",
                      &scene->materials[4], 0.3, 0.3, 0.005, PI / 2, 0.6, "./material/dragon_blue.bmp");

    scene->addLight(Vector3f(0, -1, -0.2), -2.5, Point(0, 1.9, 0.85), 0.15, &scene->materials[11]);
    camera->render(20000, 1e6);
}

void renderParametricSurfaceScene()
{
    int sceneNum = 1;
    Scene *scene = new Scene(sceneNum);
    Camera *camera = new Camera(0.684, 0.811, 3e-3, 0.47, 2048, 1536, Point(0, 0.25, -0.5 + EPS), scene);
    scene->addWall(sceneNum);
    scene->addParametricSurface(Vector3f(-0.35, -0.46, 0.27), 0.15, ParmetricSurface::SurfaceType::TORUS, &scene->materials[1]);
    scene->addParametricSurface(Vector3f(0.1, -0.226, 0), 0.1, ParmetricSurface::SurfaceType::TREFOIL, &scene->materials[1]);
    scene->addLight(Vector3f(0, -1, 0), -1 + EPS, Point(0, 1 - EPS, -0.1), 0.1, &scene->materials[11]);
    camera->render(10000, 1e6);
}

int main()
{
    //freopen("log.txt", "w", stdout);
    renderDragonScene();
    //renderParametricSurfaceScene();
    return 0;
}

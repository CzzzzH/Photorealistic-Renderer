#ifndef CAMERA_H

#define CAMERA_H

#include "util.hpp"
#include "image.h"

using namespace std;

class ViewPoint;

class Scene;

class Camera
{
public:
    int width, height;
    double fx, fy, aperture, focus;
    Color light;
    Scene *scene;
    Point source;
    vector<ViewPoint> viewPoint;
    Image img;

    Camera(double x, double y, double a, double f, int w, int h, Point s, Scene* se);
    void getIllumination(ll roundNum, ll photonsNum);
    void render(ll roundNum, ll photonsNum);
    void save(string filePath);
};

#endif // CAMERA_H

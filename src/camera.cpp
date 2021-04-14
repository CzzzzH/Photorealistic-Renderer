#include "camera.h"
#include "scene.h"
#include "vpkdtree.h"
#include "omp.h"

using namespace std;

// 初始化相机
Camera::Camera(double x, double y, double a, double f, int w, int h, Point s, Scene* se)
    : fx(x), fy(y), aperture(a), focus(f), width(w), height(h), source(s), scene(se), img(width, height)
{
    viewPoint.resize(w * h);
}

// 计算光强（论文公式）
void Camera::getIllumination(ll roundNum, ll photonsNum)
{
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            ViewPoint &vp = viewPoint[i * height + j];
            Color newColor = vp.flux / (PI * vp.r2 * roundNum * photonsNum) + light * vp.fluxLight / roundNum;         
            newColor.x() = pow(newColor.x(), GAMMA);
            newColor.y() = pow(newColor.y(), GAMMA);
            newColor.z() = pow(newColor.z(), GAMMA);
            newColor = Vector3f::min(newColor, Vector3f(1, 1, 1));
            /*
            cout << "[Color " << i << " " << j << " ] ";
            newColor.print();
            */
            img.SetPixel(i, j, newColor);
        }
}

// 渲染主函数
void Camera::render(ll roundNum, ll photonsNum)
{
    // 构建用于加速求交的物体KD树
    scene->buildObjTree();
    cout << "Complete buliding Object KDTree !" << endl;

    int cx = width / 2, cy = height / 2;
    Plane focusPlane(Vector3f(0, 0, 1), source.z() + focus, nullptr);

    // 设置灯光颜色和初始颜色权重
    light = Color(2, 2, 2);
    Color initWeight = Color(2, 2, 2);

    // SPPM 主迭代循环
    for (ll iter = 0; iter < roundNum; ++iter)
    {
        cout << "Now Iter: " << iter << endl;

        // 先通过 Ray Tracing 确定 View Points 的位置
        for (int i = 0; i < width; ++i)
        {
            #pragma omp parallel for schedule(dynamic, 1), num_threads(12)
            for (int j = 0; j < height; ++j)
            {
                Point absoluteDir(double(i - cx) / width / fx, double(j - cy) / height / fy, 0);
                Ray cameraRay(source, absoluteDir - source);
                Hit focusHit;
                focusPlane.intersect(cameraRay, focusHit, 0);
                Point focusPoint = cameraRay.pointAtParameter(focusHit.t);
                // 利用光圈和焦平面实现景深效果
                double theta = Rand::randomRegular(0, 2 * PI);
                Point newSource = cameraRay.getOrigin() + Point(cos(theta), sin(theta), 0) * aperture;
                Point newDir = focusPoint - newSource;
                viewPoint[i * height + j].dir = newDir.normalized() * (-1);
                /*
                cout << "[Camera Ray] " << endl;
                cout << "Source: ";
                newSource.print();
                cout << "Dir: ";
                newDir.print();
                */
                cameraRay = Ray(newSource, newDir);
                Color weight(1, 1, 1);
                scene->trace(cameraRay, weight, 1 , iter * (photonsNum + width * height)
                             + i * height + j, &viewPoint[i * height + j]);
            }
        }

        if (scene->vpTree) delete scene->vpTree;
        scene->vpTree = new VpKDTree(this);

        // 再利用光子随机产生光线然后送光
        #pragma omp parallel for schedule(dynamic, 1), num_threads(12)
        for (int i = 0; i < photonsNum; ++i)
        {
            Ray ray = scene->generateRay(iter * photonsNum + (iter + 1) * width * height + i);
            scene->trace(ray, initWeight * light, 1, iter * photonsNum + i, nullptr);
        }

        // 每隔一定迭代数输出结果图片
        if ((iter + 1) % ITER_INTERVAL == 0)
        {
            getIllumination(iter + 1, photonsNum);
            string filePath = "./result/result-" + to_string((iter + 1) / ITER_INTERVAL) + ".bmp";
            img.SaveBMP(filePath.c_str());
        }
    }

    // 保存最终结果
    getIllumination(roundNum, photonsNum);
    string filePath = "./result/final_result.bmp";
    img.SaveBMP(filePath.c_str());
}

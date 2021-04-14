#include "scene.h"
#include "viewpoint.hpp"

// 初始化各种材质
Scene::Scene(int sceneNum)
{
    if (sceneNum == 0)
    {
        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0, 0, 1, 0, 0, 0, 1.65, BRDF::Type::GLASS)));
        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0, 0, 1, 0, 0, 0, 1.65, BRDF::Type::GLASS)));
        materials.push_back(Material(Color(1, 0.9, 0.2), "", BRDF(0, 0.1, 0.9, 1, 0, 0, 1.55, BRDF::Type::BUNNY)));
        materials.push_back(Material(Color(1, 0.4, 0.4), "", BRDF(0, 0.1, 0.9, 1, 0.5, 20, 1.65, BRDF::Type::GLASS)));

        //materials.push_back(Material(Color(1, 1, 0.7), "", BRDF(0, 1, 0, 0.9, 0.1, 10, 1, BRDF::Type::Dragon)));
        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0.15, 0.85, 0, 0.8, 0.2, 20, 0, BRDF::Type::DRAGON)));

        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0.5, 0.5, 0, 0.07, 0.93, 0, 0, BRDF::Type::MIRROR)));
        //materials.push_back(Material(Color(0.7, 0.3, 0.3), "", BRDF(0, 01, 0, 0.7, 0, 0, 0, BRDF::Type::DIFFUSE)));
        materials.push_back(Material(Color(1, 1, 0.5), "./material/Ceil.bmp",
            BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::CEIL), 1 , -0.8, 0.8, -0.5, 1));
        materials.push_back(Material(Color(1, 1, 0.5), "./material/Floor.bmp",
            BRDF(0.15, 0.85, 0, 0.85, 0.15, 0, 0, BRDF::Type::FLOOR), 1 , -0.8, 0.8, -0.5, 1));
        materials.push_back(Material(Color(1, 1, 0.5), "./material/Wall.bmp",
            BRDF(0.05, 0.95, 0, 0.95, 0.05, 0, 0, BRDF::Type::WALL), 2 , -0.5, 2, -0.5, 1));
        materials.push_back(Material(Color(1, 1, 0.5), "./material/Wall.bmp",
            BRDF(0.05, 0.95, 0, 0.95, 0.05, 0, 0, BRDF::Type::WALL), 2 , -0.5, 2, -0.5, 1));
        materials.push_back(Material(Color(1, 1, 0.5), "./material/BackWall.bmp",
            BRDF(0.05, 0.95, 0, 0.95, 0.05, 0, 0, BRDF::Type::WALL), 0 , -0.8, 0.8, -0.5, 2));

        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0, 1, 0, 0, 0, 0, 0, BRDF::Type::LIGHT)));

        materials.push_back(Material(Color(1, 1, 0.8), "", BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::WALL)));
    }
    else
    {
        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0, 0, 1, 0, 0, 0, 1.65, BRDF::Type::GLASS)));
        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0, 0, 1, 0, 0, 0, 1.65, BRDF::Type::GLASS)));
        materials.push_back(Material(Color(0.8, 0.9, 1), "", BRDF(0, 0.1, 0.9, 1, 0, 0, 1.55, BRDF::Type::GLASS)));
        materials.push_back(Material(Color(0.8, 0.9, 1), "", BRDF(0, 0.1, 0.9, 1, 0, 0, 1.55, BRDF::Type::GLASS)));

        //materials.push_back(Material(Color(1, 1, 0.7),     "", BRDF(0, 1, 0, 0.9, 0.1, 10, 1, BRDF::Type::Dragon)));
        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0.15, 0.85, 0, 0.8, 0.2, 20, 0, BRDF::Type::DRAGON)));

        materials.push_back(Material(Color(1, 0.7, 0.3), "", BRDF(0, 01, 0, 0.7, 0, 0, 0, BRDF::Type::WALL)));
        materials.push_back(Material(Color(0.8, 0.5, 0.5), "",
            BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::CEIL)));
        materials.push_back(Material(Color(0.8, 0.3, 1), "",
            BRDF(0.2, 0.8, 0, 0.8, 0.2, 0, 0, BRDF::Type::FLOOR)));
        materials.push_back(Material(Color(0.7, 0.4, 0.3), "",
            BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::WALL)));
        materials.push_back(Material(Color(0.7, 0.4, 0.3), "",
            BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::WALL)));
        materials.push_back(Material(Color(0.7, 0.3, 0.4), "",
            BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::WALL)));

        materials.push_back(Material(Color(1, 1, 1), "", BRDF(0, 1, 0, 0, 0, 0, 0, BRDF::Type::LIGHT)));

        materials.push_back(Material(Color(1, 1, 0.8), "", BRDF(0, 1, 0, 1, 0, 0, 0, BRDF::Type::WALL)));
    }
}

Scene::~Scene()
{
    for (int i = 0; i < objList.size(); ++i)
        delete objList[i];
}

void Scene::addTriangle(Point a, Point b, Point c, Material *m)
{
    objList.push_back(new Triangle(a, b, c, m, nullptr));
}

void Scene::addSphere(Point center, double radius, Material *m)
{
    objList.push_back(new Sphere(center, radius, m));
}

// 导入PLY格式的MESH
void Scene::addPlyMesh(string filePath, Material *m, double centerX, double centerZ, double size, double rotateXZ, double height, string uvTexturePath)
{
    Mesh *mesh = new Mesh(m);
    objList.push_back(mesh);
    fstream f(filePath, ios::in);
    Image *uvImage = nullptr;
    string token;
    vector<double> uList, vList;
    int vertexSize, faceSize, startIndex = 0;
    while (f >> token)
    {
        if (token == "vertex")
        {
            f >> vertexSize;
            mesh->vertex.resize(vertexSize);
            uList.resize(vertexSize);
            vList.resize(vertexSize);
        }
        if (token == "face")
        {
            f >> faceSize;
            startIndex = objList.size();
            objList.resize(startIndex + faceSize);
        }
        if (token == "end_header") break;
    }
    double x, y, z, bottomY = INF;
    int indexX, indexY, indexZ;
    // 根据参数构造顶点
    for (int i = 0; i < mesh->vertex.size(); ++i)
    {
        f >> x >> y >> z >> uList[i] >> vList[i];
        mesh->vertex[i] = Vector3f(x, y, z) * size;
        bottomY = min(bottomY, mesh->vertex[i].y());
    }
    mesh->getCenterPoint();

    // 旋转（沿Y轴）
    for (int i = 0; i < mesh->vertex.size(); ++i)
    {
        Vector3f temp = mesh->vertex[i] - mesh->centerPoint;
        mesh->vertex[i] = mesh->centerPoint + Vector3f(cos(rotateXZ) * temp.x() - sin(rotateXZ) * temp.z(),
                                                       temp.y(),
                                                       sin(rotateXZ) * temp.x() + cos(rotateXZ) * temp.z());
    }

    mesh->getCenterPoint();

    // 移动
    Vector3f moveVec(centerX - mesh->centerPoint.x(), height + FLOOR_HEIGHT - bottomY, centerZ - mesh->centerPoint.z());
    for (int i = 0; i < mesh->vertex.size(); ++i)
        mesh->vertex[i] += moveVec;

    cout << "Mesh Center: ";
    mesh->centerPoint.print();

    mesh->getMaxPoint();
    mesh->getMinPoint();
    mesh->getCenterPoint();

    cout << "Mesh Min: ";
    mesh->minPoint.print();
    cout << "Mesh Max: ";
    mesh->maxPoint.print();
    
    // 生成面片
    if (uvTexturePath != "") uvImage = Image::LoadBMP(uvTexturePath.c_str());
    mesh->face.resize(faceSize);
    for (int i = startIndex; i < objList.size(); ++i)
    {
        f >> token >> indexX >> indexY >> indexZ;
        Triangle *tri = new Triangle(mesh->vertex[indexX], mesh->vertex[indexY], mesh->vertex[indexZ], m, mesh);
        if (uvTexturePath != "")
        {
            tri->verticesColor[0] = uvImage->GetPixel(uList[indexX] * uvImage->Width(), vList[indexX] * uvImage->Height() - EPS);
            tri->verticesColor[1] = uvImage->GetPixel(uList[indexY] * uvImage->Width(), vList[indexY] * uvImage->Height() - EPS);
            tri->verticesColor[2] = uvImage->GetPixel(uList[indexZ] * uvImage->Width(), vList[indexZ] * uvImage->Height() - EPS);
            tri->colorUsed = true;
        }
        else tri->colorUsed = false;
        mesh->face[i - startIndex] = tri;
        objList[i] = tri;
    }
    cout << "Add Mesh Sucessfully! " << endl;
    if (uvImage) delete uvImage;
}

void Scene::addParametricSurface(Vector3f offset, double radius, ParmetricSurface::SurfaceType tp, Material *m)
{
    objList.push_back(new ParmetricSurface(offset, radius, tp, m));
}

void Scene::addLight(Vector3f normal, double d, Point center, double radius, Material *m)
{
    objList.push_back(new Disc(normal, d, center, radius, m));
    lightSource = center;
    lightNormal = normal.normalized();
    lightRadius = radius;
}

void Scene::addWall(int sceneNum)
{
    Point *triVertex;
    if (sceneNum == 0)
    {
        Point temp[8] =
        {
            Point(-0.8, 2, -0.5),
            Point(-0.8, 2, 1),
            Point(0.8, 2, 1),
            Point(0.8, 2, -0.5),
            Point(-0.8, -0.5, -0.5),
            Point(-0.8, -0.5, 1),
            Point(0.8, -0.5, 1),
            Point(0.8, -0.5, -0.5),

        };
        triVertex = temp;
     }
     else
     {
        Point temp[8] =
        {
            Point(-0.8, 1, -0.5),
            Point(-0.8, 1, 0.5),
            Point(0.8, 1, 0.5),
            Point(0.8, 1, -0.5),
            Point(-0.8, -0.5, -0.5),
            Point(-0.8, -0.5, 0.5),
            Point(0.8, -0.5, 0.5),
            Point(0.8, -0.5, -0.5),

        };
        triVertex = temp;
     }

    // 前墙（面对）
    addTriangle(triVertex[1], triVertex[2], triVertex[5], &materials[5]);
    addTriangle(triVertex[2], triVertex[5], triVertex[6], &materials[5]);

    // 天花板
    addTriangle(triVertex[0], triVertex[1], triVertex[2], &materials[6]);
    addTriangle(triVertex[0], triVertex[2], triVertex[3], &materials[6]);

    // 地板
    addTriangle(triVertex[4], triVertex[5], triVertex[6], &materials[7]);
    addTriangle(triVertex[4], triVertex[6], triVertex[7], &materials[7]);

    // 左墙
    addTriangle(triVertex[0], triVertex[1], triVertex[4], &materials[8]);
    addTriangle(triVertex[1], triVertex[4], triVertex[5], &materials[8]);

    // 右墙
    addTriangle(triVertex[2], triVertex[3], triVertex[6], &materials[9]);
    addTriangle(triVertex[3], triVertex[6], triVertex[7], &materials[9]);

    // 后墙（背对）
    addTriangle(triVertex[0], triVertex[4], triVertex[7], &materials[10]);
    addTriangle(triVertex[0], triVertex[3], triVertex[7], &materials[10]);
}

// 构造物体求交的KD树
void Scene::buildObjTree()
{
    Point lowerBound = Vector3f(INF);
    Point upperBound = Vector3f(-INF);
    for (int i = 0; i < objList.size(); ++i)
    {
        lowerBound = Vector3f::min(lowerBound, objList[i]->minPoint);
        upperBound = Vector3f::max(upperBound, objList[i]->maxPoint);
    }
    objTree.root = objTree.build(0, 0, objList, lowerBound, upperBound);
}

// 产生光线


Ray Scene::generateRay(ll iter)
{
    double alpha = Rand::randomQMC(0, 2 * PI,  32 + int(iter / 1e8), iter);
    double alphaLen = Rand::randomQMC(0, lightRadius, 42 + int(iter / 1e8), iter);
    Vector3f offset = Vector3f::ZERO;
    offset.x() += sin(alpha) * alphaLen;
    offset.y() += cos(alpha) * Vector3f::cross(Vector3f(0, 1, 0), lightNormal).length() * alphaLen;
    offset.z() += cos(alpha) * Vector3f::cross(Vector3f(0, 0, 1), lightNormal).length() * alphaLen;
    Point source = lightSource + offset;
    Vector3f dir = randomReflectedRay(lightNormal, 0, 1, iter);
    return Ray(source + dir * EPS, dir);
}

// 产生光线
/*
Ray Scene::generateRay(ll iter)
{
    double alpha = Rand::randomRegular(0, 2 * PI);
    Point source = lightSource + Vector3f(cos(alpha), 0, sin(alpha)) * lightRadius;
    Vector3f dir = randomReflectedRay(lightNormal, 0, 1, iter);
    return Ray(source + dir * EPS, dir);
}
*/

// 对发射光线做随机扰动
Vector3f Scene::randomReflectedRay(Vector3f normal, int depth, double shiniess, ll iter)
{
    Vector3f u, v;
    double theta, phi;
    u = Vector3f::cross(Vector3f(1, 0, 0), normal);
    if (u.length() * u.length() < EPS) u = Vector3f::cross(Vector3f(0, 1, 0), normal);
    u.normalize();
    v = Vector3f::cross(normal, u);
    v.normalize();
    theta = Rand::randomQMC(0, 2 * PI, 2 * depth + 1 + int(iter / 1e8),  iter);
    phi = asin(pow(Rand::randomQMC(0, 1, 2 * depth + 2 + int(iter / 1e8), iter), 1.0 / (shiniess + 1)));
    return (normal * cos(phi) + (u * cos(theta) + v * sin(theta)) * sin(phi));
}

// 光线追踪主体函数
void Scene::trace(const Ray &ray, const Color &weight, int depth, ll iter, ViewPoint *vp)
{
    if (depth > MAX_TRACE_DEPTH) return;
    Hit hit;

    // 在KD树上求交
    objTree.intersect(objTree.root, ray, hit, 1e-3);
    if (hit.hitObj == nullptr) return;

    Point hitPoint = ray.pointAtParameter(hit.t);
    BRDF &hitBRDF = hit.hitObj->material->brdf;
    Vector3f &normal = hit.normal;
    Color hitColor;
    if (hit.hitObj->colorUsed)
    {
        auto tri = dynamic_cast<Triangle*>(hit.hitObj);
        Point &v1 = tri->vertices[0], &v2 = tri->vertices[1], &v3 = tri->vertices[2];
        double w1 = ((v2.y() - v3.y()) * (hitPoint.x() - v3.x()) + ((v3.x() - v2.x()) * (hitPoint.y() - v3.y())))
                    / ((v2.y() - v3.y()) * (v1.x() - v3.x()) + ((v3.x() - v2.x()) * (v1.y() - v3.y())));
        double w2 = ((v3.y() - v1.y()) * (hitPoint.x() - v3.x()) + ((v1.x() - v3.x()) * (hitPoint.y() - v3.y())))
                    / ((v2.y() - v3.y()) * (v1.x() - v3.x()) + ((v3.x() - v2.x()) * (v1.y() - v3.y())));
        double w3 = 1 - w1 - w2;
        hitColor = (tri->verticesColor[0] * w1 + tri->verticesColor[1] * w2 + tri->verticesColor[2] * w3)
                    / (w1 + w2 + w3);
    }
    else hitColor = hit.hitObj->material->getColor(hitPoint);

    /*
    cout << "[Hit] t: " << hit.t << endl << "Point:  ";
    hitPoint.print();
    cout << "Dir:  ";
    ray.getDirection().print();
    cout << "Material: ";
    hit.hitObj->material->getColor(hitPoint).print();
    cout << "BRDF: " << int(hitBRDF.type) << endl;
    */

    // 通过 Russian roulette 判断是否进行镜面反射、漫反射还是折射
    double actionNum = Rand::randomRegular(0, 1);
    /*
    cout << "Count: " << ++traceCount << " Depth: " << depth << endl;
    cout << "Normal: ";
    normal.print();
    */

    Vector3f specularDir = ray.getDirection() - normal * (2 * Vector3f::dot(ray.getDirection(), normal));

    // 镜面反射
    if (hitBRDF.specular > 0 && actionNum <= hitBRDF.specular)
    {
        Ray specularRay(hitPoint + specularDir * EPS, specularDir);
        /*
        cout << "[Specular Ray]" << endl;
        cout << "Source: ";
        hitPoint.print();
        cout << "Dir: ";
        specularDir.print();
        */
        trace(specularRay, weight * hitColor, depth + 1, iter, vp);
        return;
    }
    actionNum -= hitBRDF.specular;

    // 漫反射
    if (hitBRDF.diffuse > 0 && actionNum <= hitBRDF.diffuse)
    {
        if (vp) // 当前处于 Ray Tracing Pass 阶段，初始化 ViewPoint
        {
            vp->valid = true;
            vp->pos = hitPoint;
            vp->weight = weight * hitColor;
            if (hitBRDF.type == BRDF::Type::LIGHT)
            {
                vp->valid = false;
                vp->fluxLight += vp->weight;
            }
            vp->material = hit.hitObj->material;
            vp->normal = normal;
        }
        else // 当前处于 Photons Tracing Pass 阶段
        {
            double randNum = Rand::randomRegular(0, 1);

            // 利用 Phong 模型，依旧使用 Russian roulette 选择吸收或者反射
            if (randNum <= hitBRDF.rhoS) // 不存在吸收部分, 完全镜面反射
            {
                // 下一次反射方向也进行随机化调整（SPPM的特点）
                Vector3f randomDir = randomReflectedRay(specularDir, depth, hitBRDF.phongS, iter);
                Ray randomRay(hitPoint + randomDir * EPS, randomDir);
                /*
                cout << "[Random Specular Ray]" << endl;
                cout << "Source: ";
                hitPoint.print();
                cout << "Dir: ";
                randomDir.print();
                */
                trace(randomRay, weight * hitColor, depth + 1, iter, vp);
            }
            else // 存在吸收部分和漫反射部分
            {
                randNum -= hitBRDF.rhoS;
                vpTree->update(vpTree->root, hitPoint, weight, ray.getDirection());
                if (randNum <= hitBRDF.rhoD)
                {
                    // 下一次反射方向也进行随机化调整（SPPM的特点）
                    Vector3f randomDir = randomReflectedRay(normal, depth, 1, iter);
                    if (Vector3f::dot(randomDir, normal) < 0) randomDir *= (-1);
                    Ray randomRay(hitPoint + randomDir * EPS, randomDir);
                    /*
                    cout << "[Random Absorb Ray]" << endl;
                    cout << "Source: ";
                    hitPoint.print();
                    cout << "Dir: ";
                    randomDir.print();
                    */
                    trace(randomRay, weight * hitColor, depth + 1, iter, vp);
                }
            }
        }
        return;
    }
    actionNum -= hitBRDF.diffuse;

    // 折射
    if (hitBRDF.refraction > 0 && actionNum <= hitBRDF.refraction)
    {
        double refractiveIndex = hitBRDF.refractiveIndex;

        // 判断是射入还是射出（折射率因此不同）
        if (hit.inside) refractiveIndex = 1.0 / refractiveIndex;
        double cosThetaIn = Vector3f::dot(ray.getDirection(), normal) * (-1);
        double cosThetaOut = 1 - (1 - cosThetaIn * cosThetaIn) / (refractiveIndex * refractiveIndex);

        if (cosThetaOut >= -EPS) // 如果不是全反射
        {
            cosThetaOut = sqrt(cosThetaOut);
            // Schlick's Approximation
            double R0 = pow((1 - refractiveIndex) / (1 + refractiveIndex), 2);
            double cosTheta = hit.inside ? cosThetaOut : cosThetaIn;
            double R = R0 + (1 - R0) * pow(1 - cosTheta, 5);

            // 仍然通过 Russian roulette 选择反射或折射
            if (Rand::randomRegular(0, 1) <= R)
            {
                Ray newRay(hitPoint + specularDir * EPS, specularDir);
                /*
                cout << "[Part Specular Ray]" << endl;
                cout << "Source: ";
                hitPoint.print();
                cout << "Dir: ";
                specularDir.print();
                */
                trace(newRay, weight * hitColor, depth + 1, iter, vp);
            }
            else
            {
                Vector3f refractionDir = ray.getDirection() / refractiveIndex + normal * (cosThetaIn / refractiveIndex - cosThetaOut);
                Ray newRay(hitPoint + refractionDir * EPS, refractionDir);
                /*
                cout << "[Refraction Ray]" << endl;
                cout << "Source: ";
                hitPoint.print();
                cout << "Dir: ";
                refractionDir.print();
                */
                trace(newRay, weight * hitColor, depth + 1, iter, vp);
            }
        }
        else // 全反射
        {
            Ray newRay(hitPoint + specularDir * EPS, specularDir);
            /*
            cout << "[Full Specular Ray]" << endl;
            cout << "Source: ";
            hitPoint.print();
            cout << "Dir: ";
            specularDir.print();
            */
            trace(newRay, weight * hitColor, depth + 1, iter, vp);
        }
    }
}



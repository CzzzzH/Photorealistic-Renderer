#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "util.hpp"
#include "brdf.hpp"
#include "texture.hpp"

using namespace std;

class Material
{
public:
    Texture map;
    Color color;
    BRDF brdf;
    bool mapUsed;
    int plane;
    double minX, maxX, minY, maxY;
    Material() = delete;
    Material(Color c, string mapPath, BRDF b, int plane = 0, double minX = 0, double maxX = 0, double minY = 0, double maxY = 0)
        : map(mapPath), color(c), brdf(b)
    {
        if (mapPath == "") mapUsed = false;
        else
        {
             mapUsed = true;
             this->plane = plane;
             this->minX = minX;
             this->maxX = maxX;
             this->minY = minY;
             this->maxY = maxY;
        }
    }

    Color getColor(Point point)
    {
        if (mapUsed)
        {
            int x, y;
            switch (plane)
            {
                case 0: // XY平面
                {
                    x = int(map.width * (point.x() - minX) / (maxX - minX));
                    y = int(map.height * (point.y() - minY) / (maxY - minY));
                    break;
                }
                case 1: // XZ平面
                {
                    x = int(map.width * (point.x() - minX) / (maxX - minX));
                    y = int(map.height * (point.z() - minY) / (maxY - minY));
                    break;
                }
                case 2: // YZ平面
                {
                    x = int(map.width * (point.y() - minX) / (maxX - minX));
                    y = int(map.height * (point.z() - minY) / (maxY - minY));
                    break;
                }
            }
            x = min(map.width - 1, max(x, 0));
            y = min(map.height - 1, max(y, 0));
            return map.color->GetPixel(x, y);
        }
        else return color;
    }
};

#endif // MATERIAL_HPP

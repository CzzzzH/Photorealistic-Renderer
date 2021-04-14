#ifndef UTIL_HPP
#define UTIL_HPP

#include <Vector3f.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ctime>
#include <random>
#include <cfloat>

#define INF 1e3
#define HINF 1e2
#define EPS 1e-6
#define NEWTON_DIS 1.5e-7
#define ALPHA 0.7
#define GAMMA 0.5
#define MAX_OBJ_NUM 8
#define MAX_OBJ_TREE_DEPTH 24
#define MAX_QUAD_TREE_DEPTH 12
#define DISCRETE_NUM 5
#define MAX_TRACE_DEPTH 10
#define ITER_INTERVAL 1
#define INIT_RADIUS 1e-3
#define FLOOR_HEIGHT -0.5
#define PI 3.14159265358979323846264338327950

using Color = Vector3f;
using Point = Vector3f;
using namespace std;

typedef long long ll;

namespace Prime
{
    const int prime[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
                       59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
                       127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191,
                       193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263};
}

class Rand
{
public:
    static double randomQMC(double left, double right, int base, ll iter)
    {
        double f = 1, res = 0;
        base = Prime::prime[base];
        while (iter > 0)
        {
            f /= base;
            res += f * (iter % base);
            iter /= base;
        }
        return left + res * (right - left);
    }
    static double randomRegular(double left, double right)
    {
        static mt19937 *generator = nullptr;
        if (!generator)
            generator = new mt19937(clock());
        static uniform_real_distribution<> dis(0, 1);
        return left + dis(*generator) * (right - left);
    }
};

#endif // UTIL_HPP

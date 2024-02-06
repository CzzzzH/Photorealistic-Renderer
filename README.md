## Photorealistic Renderer

An implementation of [Stochastic progressive photon mapping](https://dl.acm.org/doi/abs/10.1145/1661412.1618487) with C++



### Introduction

This project implements a C++ program for realistic scene rendering. The rendering algorithm is based on [Stochastic Progressive Photon Mapping](https://dl.acm.org/doi/abs/10.1145/1661412.1618487) (SPPM). The renderer can render mesh with textures and parametric surfaces (intersections are solved by iterative analytic equations), and the depth of field can be achieved by simulating focal length.



### Implementation

The renderer uses two KD-Trees and a quadtree to accelerate the process of rendering. The former is used for mesh intersection acceleration, and the latter is used for parametric surfaces intersection acceleration. The OpenMP is used for trivial parallel computing acceleration.

The parametric surfaces are calculated with Newton iterative method, the quadtree is important because the convergence of this method depends much on the initial value, so the ray need first intersect the bounding box of the parametric surface, which is built on quadtree.

The rendering of texture is implemented by two methods. The first method is matching the pixels of the picture to a single triangular surface, which is simple but just works with simple objects (e.g. walls). The second method is carried out by UV unfolding, by which every vertex has its color, so the color of every points on the surface can be calculated by the interpolation on the center of gravity.

Depth of field is easy to implement as the expansion of a function. Change the source point from the origin to a disk (aperture) and random sample on it, then make the light pass the focal point before it ejects. After that operation, multiple points outside the focal plane will correspond to one pixel, and there is an effect of depth of field.



### Results

There are two demo scenes rendered by this toy renderer.

The first scene depicts a palace with a big mirror, where a rabbit and two dragons are doing something with two crystal balls. (It is like a game scene in my mind , though I have not actually considered about the story about it :D)

![1](results/1.bmp)

The second scene is indeed a test of the parametric surface and the simulation of depth of field, which is a lot trivial.

![2](results/2.bmp)

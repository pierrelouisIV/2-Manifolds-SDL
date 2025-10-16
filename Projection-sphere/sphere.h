#ifndef H_SPHERE
#define H_SPHERE

#include "manifold.h"
#include "constant.h"

//
typedef struct {
    float xc;
    float yc;
    float zc;
    float r;
} Sphere;

//
void sphere(float theta, float phi, float *x, float *y, float *z);
void draw_sphere_rotated(SDL_Renderer *renderer, float pas_long, float pas_lat, float alpha, float beta, float _gamma, float d);

#endif

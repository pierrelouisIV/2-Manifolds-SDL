#ifndef H_ICOSAHEDRON
#define H_ICOSAHEDRON

#include <SDL2/SDL.h>
#include <math.h>
#include "constant.h"

// Vecteurs
typedef struct {
    float x;
    float y;
    float z;
} Vect3;

typedef struct {
    float a, b, c;
} Face; // Pas nécessaire mais bon..


// Utils
void project_point(float x, float y, float z, int *X, int *Y, float d);
void normalize(float *x, float *y, float *z);

/*          Icosphere            */
void init_icosahedron(); 
void draw_icosahedron(SDL_Renderer *renderer, float scale, float d, float alpha); 
void subdivise(Vect3 **vertices, Face **faces);

#endif

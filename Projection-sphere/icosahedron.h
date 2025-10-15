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
// Redéfinition d'un vecteur mais pour les faces de l'icosaèdre
typedef struct {
    float a, b, c;
} Face;


// Utils
void project_point(float x, float y, float z, int *X, int *Y, float d);
void normalize(float *x, float *y, float *z);

/*          Icosphere            */
void create_icosahedron(Vect3 **vertices, int *vertex_count, Face **faces, int *face_count);
void draw_icosahedron(SDL_Renderer *renderer, Vect3 *vertices, int vertex_count, Face *faces, int face_count, float alpha, float beta, float gamma, float d); 
void subdivide(Vect3 **vertices, int *vertex_count, Face **faces, int *face_count);
void draw_icosphere(SDL_Renderer *renderer, Vect3 *v, Face *f, int face_count, float scale, float d, float alpha);

#endif

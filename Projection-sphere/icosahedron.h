#ifndef H_ICOSAHEDRON
#define H_ICOSAHEDRON

#include "manifold.h"

// Redéfinition d'un vecteur mais pour les faces de l'icosaèdre
typedef struct {
    float a, b, c;
} Face;

// Utils
void normalize(float *x, float *y, float *z);

/*          Icosphere            */
void create_icosahedron(Vecteur3 **vertices, int *vertex_count, Face **faces, int *face_count);
void draw_icosahedron(SDL_Renderer *renderer, Vecteur3 *vertices, int vertex_count, Face *faces, int face_count, float alpha, float beta, float gamma, float d); 
void subdivide(Vecteur3 **vertices, int *vertex_count, Face **faces, int *face_count);
void draw_icosphere(SDL_Renderer *renderer, Vecteur3 *v, Face *f, int face_count, float scale, float d, float alpha);

#endif

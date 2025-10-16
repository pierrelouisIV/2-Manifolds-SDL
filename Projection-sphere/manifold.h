#ifndef H_MANIFOLD
#define H_MANIFOLD

#include <SDL2/SDL.h>
#include "constant.h"

// Math structure
typedef struct {
    float x;
    float y;
    float z;
} Vecteur3;


// Geometry maths
void project_point(float x, float y, float z, int *X, int *Y, float d);
void project_point_ray(float x, float y, float z, int *X, int *Y, float d);
float length(Vecteur3 v);
void gd_normalize(Vecteur3 *v);
float dot(Vecteur3 a, Vecteur3 b);
Vecteur3 cross(Vecteur3 a, Vecteur3 b);
Vecteur3 add(Vecteur3 a, Vecteur3 b);
Vecteur3 sub(Vecteur3 a, Vecteur3 b);
Vecteur3 scale(Vecteur3 *a, float s);
void scaleIn(Vecteur3 *a, float s);
void update_ray(Vecteur3 *pos, Vecteur3 *dir, float R, float dt);
Vecteur3 tangent_direction(Vecteur3 point, Vecteur3 axis);

// SDL
void draw_point(SDL_Renderer *renderer, Vecteur3 position, float d);
void debug_draw_vector(SDL_Renderer *renderer, Vecteur3 pos, Vecteur3 dir, float d, float s);
#endif

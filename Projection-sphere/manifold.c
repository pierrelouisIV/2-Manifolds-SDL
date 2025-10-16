#include <math.h>
#include "manifold.h"

// fonction de projection
void project_point(float x, float y, float z, int *X, int *Y, float d) {
    float xp = (x * d) / z;
    float yp = (y * d) / z;
    // screen conversion
    *X = LARGEUR / 2 + (int)xp;
    *Y = HAUTEUR / 2 - (int)yp;
}
void project_point_ray(float x, float y, float z, int *X, int *Y, float d) {
    float xp = (x * d) / (z + d);
    float yp = (y * d) / (z + d);
    *X = LARGEUR / 2 + (int)xp;
    *Y = HAUTEUR / 2 - (int)yp;
}
//
float length(Vecteur3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void gd_normalize(Vecteur3 *v) {
    float len = length(*v);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}
//
float dot(Vecteur3 a, Vecteur3 b) {
       return (a.x * b.x + a.y * b.y + a.z * b.z);
}
//
Vecteur3 cross(Vecteur3 a, Vecteur3 b) {
    Vecteur3 c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}
//
Vecteur3 add(Vecteur3 a, Vecteur3 b) {
    Vecteur3 res = {a.x + b.x, a.y + b.y, a.z + b.z};
    return res;
}
//
Vecteur3 sub(Vecteur3 a, Vecteur3 b) {
    Vecteur3 res = {a.x - b.x, a.y - b.y, a.z - b.z};
    return res;
}
//
Vecteur3 scale(Vecteur3 *a, float s) {
    Vecteur3 res = {a->x *= s, a->y *= s, a->z *= s};
    return res;
}
void scaleIn(Vecteur3 *a, float s) {
    a->x *= s;
    a->y *= s;
    a->z *= s;
}
//
void update_ray(Vecteur3 *pos, Vecteur3 *dir, float R, float dt) {
    //
    gd_normalize(pos);
    scale(pos, R);
    gd_normalize(dir);
    
    Vecteur3 axis = cross(*pos, *dir);
    gd_normalize(&axis);

    float angle = dt / R;
    Vecteur3 pos_rot;
    // Formule de rotation : P' = P cos(theta) + (axis * P) sin(theta) + axis(axis*P)(1-cos(theta))
    Vecteur3 cross_p = cross(axis, *pos);
    scaleIn(&cross_p, sin(angle));
    Vecteur3 p_cos = *pos;
    scaleIn(&p_cos, cos(angle));
    pos_rot = add(p_cos, cross_p);
    gd_normalize(&pos_rot);
    scaleIn(&pos_rot, R);
    // dir = axis * pos
    Vecteur3 new_dir = cross(axis, pos_rot);
    gd_normalize(&new_dir);

    /* Intégration d'Euler ?
    float dir_len = length(*dir);
    Vecteur3 d_dir = *pos;
    scaleIn(&d_dir, -(dir_len * dir_len / (R * R)));   // -|D|² / R² * P
    // P(t + dt) = P + D * dt
    Vecteur3 delta_p = *dir;
    scaleIn(&delta_p, dt);
    Vecteur3 new_pos = add(*pos, delta_p);
    // D(t+dt) = D + dD * dt
    scaleIn(&d_dir, dt);
    Vecteur3 new_dir = add(*dir, d_dir);
    // corrections
    gd_normalize(&new_pos);
    scale(&new_pos, R);
    // caclul de la tangente (dir perpendiculaire à pos)
    new_dir = tangent_direction(new_pos, new_dir);
    */
    *pos = pos_rot;
    *dir = new_dir;
}
//
Vecteur3 tangent_direction(Vecteur3 point, Vecteur3 axis) {
    Vecteur3 n = point;
    gd_normalize(&n);  // normale à la sphère : N = P / ||P||

    //  Projection tangentielle : D = A - (A*N)N
    float tmp = dot(axis, n);
    Vecteur3 proj = n;
    scale(&proj, tmp);
    Vecteur3 tangent = sub(axis, proj);
    gd_normalize(&tangent);    // normaliser D
    return tangent;
}

// SDL functions
void draw_point(SDL_Renderer *renderer, Vecteur3 position, float d) {
    if (position.z <= 0.1f) return; //point derrière la caméra
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    int X, Y;
    project_point_ray(position.x, position.y, position.z, &X, &Y, d);
    SDL_RenderDrawPoint(renderer, X, Y);
}
// 
void debug_draw_vector(SDL_Renderer *renderer, Vecteur3 pos, Vecteur3 dir, float d, float s) {
    //
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // Point de départ
    int X1, Y1;
    project_point_ray(pos.x, pos.y, pos.z, &X1, &Y1, d);

    // Point d’arrivée (pos + dir * scale)
    Vecteur3 end = scale(&dir, s);
    Vecteur3 target = add(pos, end);

    int X2, Y2;
    project_point_ray(target.x, target.y, target.z, &X2, &Y2, d);

    //
    SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
}

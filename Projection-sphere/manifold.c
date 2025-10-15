
#include "manifold.h"
#include <math.h>

//
float length(Vecteur3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void normalize(Vecteur3 *v) {
    float len = length(*v);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}
//
float dot(Vecteur3 a, Vecteur3 b) {
       return (a.x * b.x + a.y * b.y + a.z * a.z);
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

void scale(Vecteur3 *a, float s) {
    a->x *= s;
    a->y *= s;
    a->z *= s;
}

//
void update_ray(Vecteur3 *pos, Vecteur3 *dir, float R, float dt) {
    // 1️⃣ Avancer
    Vecteur3 move = *dir;
    scale(&move, dt);
    Vecteur3 new_pos = add(*pos, move);

    // 2️⃣ Ramener sur la sphère
    normalize(&new_pos);
    scale(&new_pos, R);

    // 3️⃣ Corriger la direction pour rester tangente
    Vecteur3 normal = new_pos;   // normale = position sur la sphère
    normalize(&normal);

    float d_dot_n = dot(*dir, normal);
    Vecteur3 correction = normal;
    scale(&correction, d_dot_n);
    Vecteur3 new_dir = sub(*dir, correction);
    normalize(&new_dir);

    // 4️⃣ Sauvegarde
    *pos = new_pos;
    *dir = new_dir;
}

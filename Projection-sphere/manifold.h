#ifndef H_MANIFOLD
#define H_MANIFOLD


// Math structur
typedef struct {
    float x;
    float y;
    float z;
} Vecteur3;


//
float length(Vecteur3 v);
void normalize(Vecteur3 *v);
float dot(Vecteur3 a, Vecteur3 b);
Vecteur3 add(Vecteur3 a, Vecteur3 b);
Vecteur3 sub(Vecteur3 a, Vecteur3 b);
void scale(Vecteur3 *a, float s);
void update_ray(Vecteur3 *pos, Vecteur3 *dir, float R, float dt);

#endif

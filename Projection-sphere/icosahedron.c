
#include "icosahedron.h"
#include "constant.h"

// Utils :
// fonction de projection
void project_point(float x, float y, float z, int *X, int *Y, float d) {
    float xp = (x * d) / z;
    float yp = (y * d) / z;
    // screen conversion
    *X = LARGEUR / 2 + (int)xp;
    *Y = HAUTEUR / 2 - (int)yp;
}
void normalize(float *x, float *y, float *z) {
    // Calcul des milieux puis normalisation
    float len = sqrt((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x /= len;
    *y /= len;
    *z /= len;
}

/*----  Icosphere ------*/
float vertices[12][3] = { 
    {-1,  PHI, 0},
    { 1,  PHI, 0},
    {-1, -PHI, 0},
    { 1, -PHI, 0},
    {0, -1,  PHI},
    {0,  1,  PHI},
    {0, -1, -PHI},
    {0,  1, -PHI},
    { PHI, 0, -1},
    { PHI, 0,  1},
    {-PHI, 0, -1},
    {-PHI, 0,  1}
};
int edges[30][2] = {
    {0,1}, {0,5}, {0,7}, {0,10}, {0,11},
    {1,5}, {1,7}, {1,8}, {1,9},
    {2,3}, {2,4}, {2,6}, {2,10}, {2,11},
    {3,4}, {3,6}, {3,8}, {3,9},
    {4,5}, {4,9}, {4,11},
    {5,9}, {5,11},
    {6,7}, {6,8}, {6,10},
    {7,8}, {7,10},
    {8,9},
    {10,11}
};
int faces[20][3] = {
    {0, 11, 5},
    {0, 5, 1},
    {0, 1, 7},
    {0, 7, 10},
    {0, 10, 11},
    {1, 5, 9},
    {5, 11, 4},
    {11, 10, 2},
    {10, 7, 6},
    {7, 1, 8},
    {3, 9, 4},
    {3, 4, 2},
    {3, 2, 6},
    {3, 6, 8},
    {3, 8, 9},
    {4, 9, 5},
    {2, 4, 11},
    {6, 7, 8},
    {9, 8, 1},
    {10, 6, 2}
};


// Functions :
void init_icosahedron() {   
    // Normalisation des sommets de l'icosaèdre, pour que tous soient sur une même sphère d'unité :
    for (int i = 0; i < 12; ++i) {
        normalize(&vertices[i][0], &vertices[i][1], &vertices[i][2]);
    }
}
void draw_icosahedron(SDL_Renderer *renderer, float scale, float d, float alpha) {
    int X1, Y1, X2, Y2;

    // Boucle sur chaque arrêtes de l'icosaèdre :
    for (int i = 0; i < 30; i++) {
        int a = edges[i][0];
        int b = edges[i][1];

        // Points de départ
        float x1 = vertices[a][0] * scale;
        float y1 = vertices[a][1] * scale;
        float z1 = vertices[a][2] * scale;

        float x2 = vertices[b][0] * scale;
        float y2 = vertices[b][1] * scale;
        float z2 = vertices[b][2] * scale;
        
        // Rotation axe Y
        float x1rY = x1 * cosf(alpha) + z1 * sinf(alpha);
        float y1rY = y1;
        float z1rY = -x1 * sinf(alpha) + z1 * cosf(alpha);
        
        float x2rY = x2 * cosf(alpha) + z2 * sinf(alpha);
        float y2rY = y2;
        float z2rY = -x2 * sinf(alpha) + z2 * cosf(alpha);

        // Projection (pour la perspective)
        z1rY += 400.0f;
        z2rY += 400.0f;
        project_point(x1rY, y1rY, z1rY, &X1, &Y1, d);
        project_point(x2rY, y2rY, z2rY, &X2, &Y2, d);
        SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
    }
}

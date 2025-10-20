#include "icosahedron.h"

// Utils :
void normalize(float *x, float *y, float *z) {
    // Calcul des milieux puis normalisation
    float len = sqrt((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x /= len;
    *y /= len;
    *z /= len;
}

/*----  Icosphere ------*/
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


//
void create_icosahedron(Vecteur3 **vertices, int *vertex_count, Face **faces, int *face_count) {
    // init des sommets :
    float verts[12][3] = { 
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
    // Normalisation de tous les sommets
    for (int i = 0; i < 12; i++)
        normalize(&verts[i][0], &verts[i][1], &verts[i][2]);

    // Les 20 faces (triangles)
    int face_indices[20][3] = {
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
        {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        {4, 9, 5}, {2, 4, 11}, {6, 7, 8}, {9, 8, 1}, {10, 6, 2}
    };

    // Allocation dynamique
    *vertices = malloc(sizeof(Vecteur3) * 12);
    memcpy(*vertices, verts, sizeof(verts));
    *vertex_count = 12;

    *faces = malloc(sizeof(Face) * 20);
    for (int i = 0; i < 20; i++) {
        (*faces)[i].a = face_indices[i][0];
        (*faces)[i].b = face_indices[i][1];
        (*faces)[i].c = face_indices[i][2];
    }
    *face_count = 20;
}

// Dessine l'icosaèdre
void draw_icosahedron(SDL_Renderer *renderer, Vecteur3 *vertices, int vertex_count, Face *faces, int face_count, float alpha, float beta, float gamma, float d) {
    //
    for (int i = 0; i < face_count; i++) {
        Face f = faces[i];

        // --- Sommets du triangle ---
        Vecteur3 v[3] = {
            vertices[(int)f.a],
            vertices[(int)f.b],
            vertices[(int)f.c]
        };

        // --- Rotation sur X, Y, Z ---
        for (int j = 0; j < 3; j++) {
            float x = v[j].x;
            float y = v[j].y;
            float z = v[j].z;

            // Rotation X
            float y1 = y * cosf(beta) - z * sinf(beta);
            float z1 = y * sinf(beta) + z * cosf(beta);
            y = y1; z = z1;

            // Rotation Y
            float x1 = x * cosf(alpha) + z * sinf(alpha);
            z1 = -x * sinf(alpha) + z * cosf(alpha);
            x = x1; z = z1;

            // Rotation Z
            float x2 = x * cosf(gamma) - y * sinf(gamma);
            float y2 = x * sinf(gamma) + y * cosf(gamma);
            x = x2; y = y2;

            // Translation pour éviter la caméra au centre
            z += 1.5f;

            v[j].x = x;
            v[j].y = y;
            v[j].z = z;
        }

        // --- Projection et dessin ---
        int X1, Y1, X2, Y2, X3, Y3;
        if (v[0].z > 0 && v[1].z > 0 && v[2].z > 0) {
            project_point(v[0].x, v[0].y, v[0].z, &X1, &Y1, d);
            project_point(v[1].x, v[1].y, v[1].z, &X2, &Y2, d);
            project_point(v[2].x, v[2].y, v[2].z, &X3, &Y3, d);

            SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
            SDL_RenderDrawLine(renderer, X2, Y2, X3, Y3);
            SDL_RenderDrawLine(renderer, X3, Y3, X1, Y1);
        }
    }
}


// Division des triangles, donc de chaque face en quatre triangles
void subdivide(Vecteur3 **vertices, int *vertex_count, Face **faces, int *face_count) {
    int old_face_count = *face_count;
    Face *old_faces = *faces;

    // Allocation de mémoire  pour les nouveaux sommets + faces
    Face *new_faces = malloc(old_face_count * 4 * sizeof(Face));
    Vecteur3 *new_vertices = malloc((*vertex_count + old_face_count * 3) * sizeof(Vecteur3));
    memcpy(new_vertices, *vertices, *vertex_count * sizeof(Vecteur3));
    int new_vertex_count = *vertex_count;
    int new_face_count = 0;

    for (int i = 0; i < old_face_count; i++) {
        Vecteur3 a = new_vertices[(int)old_faces[i].a];
        Vecteur3 b = new_vertices[(int)old_faces[i].b];
        Vecteur3 c = new_vertices[(int)old_faces[i].c];

        // Calcul des milieux de chaque segments
        Vecteur3 ab = { (a.x+b.x)/2, (a.y+b.y)/2, (a.z+b.z)/2 };
        Vecteur3 bc = { (b.x+c.x)/2, (b.y+c.y)/2, (b.z+c.z)/2 };
        Vecteur3 ca = { (c.x+a.x)/2, (c.y+a.y)/2, (c.z+a.z)/2 };

        // normalisation pour la sphère
        normalize(&ab.x, &ab.y, &ab.z);
        normalize(&bc.x, &bc.y, &bc.z);
        normalize(&ca.x, &ca.y, &ca.z);

        int i_ab = new_vertex_count++;
        int i_bc = new_vertex_count++;
        int i_ca = new_vertex_count++;

        new_vertices[i_ab] = ab;
        new_vertices[i_bc] = bc;
        new_vertices[i_ca] = ca;

        // 4 nouveaux triangles
        new_faces[new_face_count++] = (Face){ old_faces[i].a, i_ab, i_ca };
        new_faces[new_face_count++] = (Face){ old_faces[i].b, i_bc, i_ab };
        new_faces[new_face_count++] = (Face){ old_faces[i].c, i_ca, i_bc };
        new_faces[new_face_count++] = (Face){ i_ab, i_bc, i_ca };
    }

    // Libération de la mémoire
    free(*faces);
    free(*vertices);
    *faces = new_faces;
    *vertices = new_vertices;
    *vertex_count = new_vertex_count;
    *face_count = new_face_count;
}

// Fonction pour dessiner l'icosphère
void draw_icosphere(SDL_Renderer *r, Vecteur3 *v, Face *f, int face_count, float scale, float d, float alpha) {
    for (int i = 0; i < face_count; i++) {
        Vecteur3 p[3];
        for (int j = 0; j < 3; j++) {
            Vecteur3 P = v[(int) (&f[i].a)[j] ]; // récupère le bon sommet
            // rotation Y
            float xr = P.x * cosf(alpha) + P.z * sinf(alpha);
            float yr = P.y;
            float zr = -P.x * sinf(alpha) + P.z * cosf(alpha);
            zr += 3.0f; // éloigne
            p[j].x = xr; p[j].y = yr; p[j].z = zr;
        }

        // Projection (pour la perspective)
        int X1, Y1, X2, Y2, X3, Y3;
        //p[0].z += 400.0f;
        //p[1].z += 400.0f;
        //p[2].z += 400.0f;
        project_point(p[0].x, p[0].y, p[0].z, &X1, &Y1, d);
        project_point(p[1].x, p[1].y, p[1].z, &X2, &Y2, d);
        project_point(p[2].x, p[2].y, p[2].z, &X3, &Y3, d);

        // trace les lignes
        SDL_RenderDrawLine(r, X1, Y1, X2, Y2);
        SDL_RenderDrawLine(r, X2, Y2, X3, Y3);
        SDL_RenderDrawLine(r, X3, Y3, X1, Y1);
    }
}

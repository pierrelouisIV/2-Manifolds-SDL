#include <stdio.h>
#include "icosahedron.h"
#include "sphere.h"

// Vars globales 
float d = 400.0f;   // distance de projection (zoom)
float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;
float alpha = 0.0f; // angle de rotation de laxe Y
float beta = 0.0f;  // angle de rotation de l'axe X
float _gamma = 0.0f; // angle de rotation de l'axe Z
float R = 150.0f;   // Rayon de la sphere


/* main function  */
int main(int argc, char **argv)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        printf("SDL_Init Error : %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL window    
    SDL_Window *window = SDL_CreateWindow("Projection Sphere", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR, HAUTEUR, 0);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Main loop flag
    int quit = 0;
    SDL_Event e;
    
    // Pour la sphere : petits pas longtitude et latitude pour la sphère en maille classique (4, 6, 12, 18, 36)
    float pas_longitude = M_PI / 18.0f;  //   environ 45°
    float pas_latitude = M_PI / 18.0f;   //   environ 45°
    
    // Création de l’icosaèdre
    Vecteur3 *vertices;
    Face *faces;
    int vertex_count, face_count;
    create_icosahedron(&vertices, &vertex_count, &faces, &face_count); // icosahèdre de niveau 0
    
    // Initialisation du rayon
    Vecteur3 pos = {R, 0, 0};
    Vecteur3 axis = {0, 1, 0};
    Vecteur3 dir = cross(axis, pos);
    gd_normalize(&dir);

    // Main loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            // Handle other events (like mouse clicking)
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        speedY += 0.01f;
                        break;
                    case SDLK_RIGHT:
                        speedY -= 0.01f;
                        break;
                    case SDLK_UP:
                        speedX += 0.01f;
                        break;
                    case SDLK_DOWN:
                        speedX -= 0.01f;
                        break;
                    case SDLK_m:
                        speedZ += 0.01f;
                        break;
                    case SDLK_l:
                        speedZ -= 0.01f;
                        break;
                    case SDLK_d:    // pour l'icosphère
                        if (face_count > 1 && face_count <  5120)
                            subdivide(&vertices, &vertex_count, &faces, &face_count);
                        
                        break;
                    // Affiner le maillage de la sphère :
                    case SDLK_x:
                        pas_longitude = M_PI / 4.0f;
                        pas_latitude  = M_PI / 4.0f;
                        break;
                    case SDLK_c:
                        pas_longitude = M_PI / 6.0f;
                        pas_latitude  = M_PI / 6.0f;
                        break;
                    case SDLK_v:
                        pas_longitude = M_PI / 12.0f;
                        pas_latitude  = M_PI / 12.0f;
                        break;
                    case SDLK_b:
                        pas_longitude = M_PI / 18.0f;
                        pas_latitude  = M_PI / 18.0f;
                        break;
                    case SDLK_n:
                        pas_longitude = M_PI / 36.0f;
                        pas_latitude  = M_PI / 36.0f;

                }
            }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Angle update
        alpha  += speedY;
        beta   += speedX;
        _gamma += speedZ;
        speedY *= 0.95f; // diminue progressivement
        speedX *= 0.95f;
        speedZ *= 0.95f;
  
        // Update the sphere
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_sphere_rotated(renderer, pas_longitude, pas_latitude, alpha, beta, _gamma, d);
        //draw_icosahedron(renderer, vertices, vertex_count, faces, face_count, alpha, beta, _gamma, d);
        
        //
        update_ray(&pos, &dir, R, 4.0f);   // 0.02f ??
        draw_point(renderer, pos, d);
        debug_draw_vector(renderer, pos, dir, d, 30.0f);
        //printf("pos: (%f, %f, %f)\n", pos.x, pos.y, pos.z);

        // Update screen
        SDL_RenderPresent(renderer);
        
        // Pour les FPS (60)
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // free the allocated memory
    SDL_Quit();
    return 0;
}

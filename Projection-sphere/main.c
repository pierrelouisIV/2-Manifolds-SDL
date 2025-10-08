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
    
    // Normaliser les sommets de l'icosaèdre
    init_icosahedron();

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

                }
            }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black screen
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
        draw_sphere_rotated(renderer, alpha, beta, _gamma, d);
        //draw_icosahedron(renderer, 100, d, alpha);

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

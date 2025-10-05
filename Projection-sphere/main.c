#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define LARGEUR 800
#define HAUTEUR 800
#define M_PI 3.14159265358979323846

// Vars globales
float sc[3] = {0.0f, 0.0f, 400.0f};  // centre de la sphere
int S_r =  150.0f;    // rayon de la sphere
float pas_long = M_PI / 18.0f; // ~10°
float pas_lat = M_PI / 18.0f; // ~10°
float d = 400.0f;   // distance de projection (zoom)

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;
float alpha = 0.0f; // angle de rotation de l'axe Y
float beta = 0.0f;  // angle de rotation de l'axe X
float _gamma = 0.0f; // angle de rotation de l'axe Z

// fonction de projection
void project_point(float x, float y, float z, int *X, int *Y, float d) {
    float xp = (x * d) / z;
    float yp = (y * d) / z;
    // screen conversion
    *X = LARGEUR / 2 + (int)xp;
    *Y = HAUTEUR / 2 - (int)yp;
}
// Generate the sphere points
void sphere(float theta, float phi, float *x, float *y, float *z) {
    *x = sc[0] + S_r * cos(theta) * sin(phi);
    *y = sc[1] + S_r * sin(theta) * sin(phi);
    *z = sc[2] + S_r * cos(phi);
}

//
void draw_sphere_rotated(SDL_Renderer *renderer) {      
    // Render the sphere
    float theta, phi;
    float x1, y1, z1, x2, y2, z2;
    int X1, Y1, X2, Y2;
    float xr1, yr1, zr1, xr2, yr2, zr2;
    // on parcours la latitude :
    for (phi = 0; phi <= M_PI; phi += pas_lat) {
        // parcours les longitudes (thetat de 0 à 2pi)
        for (theta = 0; theta < M_PI*2; theta += pas_long) {
            
            // Point courant :
            sphere(theta, phi, &x1, &y1, &z1);
            
            // rotation autour du centre :
            float dx1 = x1 - sc[0];
            float dy1 = y1 - sc[1];
            float dz1 = z1 - sc[2];
            // Rotation X
            xr1 = dx1;
            yr1 = dy1 * cos(beta) - dz1 * sin(beta);
            zr1 = dy1 * sin(beta) + dz1 * cos(beta);
            // Rotation Y
            float xr1Y = xr1 * cos(alpha) + zr1 * sin(alpha);
            float yr1Y = yr1;
            float zr1Y = -xr1 * sin(alpha) + zr1 * cos(alpha);
            // Rotation en Z
            float xr1Z = xr1Y * cos(_gamma) - yr1Y * sin(_gamma);
            float yr1Z = xr1Y * sin(_gamma) + yr1Y * cos(_gamma);
            float zr1Z = zr1Y;
            // Recentrer
            xr1Z += sc[0];
            yr1Z += sc[1];
            zr1Z += sc[2];

            // Point suivant en longitude
            float next_theta = theta + pas_long;
            if (next_theta >= M_PI*2) next_theta -= M_PI*2;
            sphere(next_theta, phi, &x2, &y2, &z2);

            // rotation
            float dx2 = x2 - sc[0];
            float dy2 = y2 - sc[1];
            float dz2 = z2 - sc[2];
            // Rotation X
            xr2 = dx2;
            yr2 = dy2 * cos(beta) - dz2 * sin(beta);
            zr2 = dy2 * sin(beta) + dz2 * cos(beta);
            // Rotation Y
            float xr2Y = xr2 * cos(alpha) + zr2 * sin(alpha);
            float yr2Y = yr2;
            float zr2Y = -xr2 * sin(alpha) + zr2 * cos(alpha);
            // Rotation Z
            float xr2Z = xr2Y * cos(_gamma) - yr2Y * sin(_gamma);
            float yr2Z = xr2Y * sin(_gamma) + yr2Y * cos(_gamma);
            float zr2Z = zr2Y;
            // Recentrer
            xr2Z += sc[0];
            yr2Z += sc[1];
            zr2Z += sc[2];
            if (zr1Z > 0 && zr2Z > 0) {
                project_point(xr1Z, yr1Z, zr1Z, &X1, &Y1, d);
                project_point(xr2Z, yr2Z, zr2Z, &X2, &Y2, d);
                SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
            }
            
            // Point suivant en latitude
            sphere(theta, phi + pas_lat, &x2, &y2, &z2);
            dx2 = x2 - sc[0];
            dy2 = y2 - sc[1];
            dz2 = z2 - sc[2];
            // Rotation X
            xr2 = dx2;
            yr2 = dy2 * cos(beta) - dz2 * sin(beta);
            zr2 = dy2 * sin(beta) + dz2 * cos(beta);
            // Rotation Y
            xr2Y = xr2 * cos(alpha) + zr2 * sin(alpha);
            yr2Y = yr2;
            zr2Y = -xr2 * sin(alpha) + zr2 * cos(alpha);
            // Rotation en Z
            xr2Z = xr2Y * cos(_gamma) - yr2Y * sin(_gamma);
            yr2Z = xr2Y * sin(_gamma) + yr2Y * cos(_gamma);
            zr2Z = zr2Y;
            // Recentrer
            xr2Z += sc[0];
            yr2Z += sc[1];
            zr2Z += sc[2];
            if (zr1Z > 0 && zr2Z > 0) {
                project_point(xr1Z, yr1Z, zr1Z, &X1, &Y1, d);
                project_point(xr2Z, yr2Z, zr2Z, &X2, &Y2, d);
                SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
            }
        }
    }
}


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
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Le clic devra modifier la caméra : zoomer un peu et se positioner sur la surface
                d += 100.0;
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
        draw_sphere_rotated(renderer);
  
        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // free the allocated memory
    SDL_Quit();
    return 0;
}

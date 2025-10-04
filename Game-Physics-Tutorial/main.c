#include <SDL2/SDL.h>
#include <stdio.h>
#include "particle_simulation.h"


//
int main(int argc, char **argv) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window *win = SDL_CreateWindow("Particle Simulator", 100, 100, WIDTH, LENGTH, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create particles
    int capacity = NUM_PARTICLES;
    Particle *particles = create_particle_array(capacity);
    int particle_count = 0;  // Start with a few particles

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
        // Handle mouse click to spawn particles
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (e.button.button == SDL_BUTTON_LEFT) {init_particle_proton(particles, particle_count, mouseX, mouseY);}
                else if (e.button.button == SDL_BUTTON_RIGHT) {init_particle_electron(particles, particle_count, mouseX, mouseY);}
                // Add new particles at mouse position
                particle_count += 1;
                PrintInformation(particle_count);
            }
        }

        // Update particles
        particles = update_particles(particles, &particle_count, &capacity);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render particles
        render_particles(renderer, particles, particle_count);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    free(particles);
    SDL_Quit();
    return 0;
}

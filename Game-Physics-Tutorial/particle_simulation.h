#ifndef PARTICLE_SIMULATION_H
#define PARTICLE_SIMULATION_H

#include <SDL2/SDL.h>
#include <math.h>
#include "constant.h"

// Two dimensionnal vector
typedef struct {
    float x;
    float y;
} Vector2;

//
typedef enum {
    REMOVED,
    NEUTRON,
    PROTON,
    ELECTRON,
    PHOTON // You can add more types if needed
} ParticleType;


// Two dimensional particle
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float mass;
    float radius;   // for collision detection
    float charge;        // Electric charge of the particle (positive or negative)
    ParticleType type;
    SDL_Color color;
    float radius_render; // for SDL rendering
    int tab_index;      // to track particle's index in the array
} Particle;

// Dynamic array
Particle* create_particle_array(int capacity);
Particle* resize_particle_array(Particle* particles, int* capacity);
Particle* update_particles(Particle* particles, int *count, int *capacity);
int is_removed_particles(Particle *p, int count);

//Prints all particles position to the output.
// We could draw them on screen in a more interesting application
void PrintParticles(Particle *particles, int count);
void PrintInformation(int count);
// Initialize a single particle at the given (x, y) position
void init_particle_neutron(Particle *p, int index, int x, int y);
void init_particle_electron(Particle *p, int index, int x, int y);
void init_particle_proton(Particle *p, int index, int x, int y);

// Electrostatic interaction
void apply_coulomb_force(Particle *p1, Particle *p2);
void apply_strong_force(Particle *p1, Particle *p2);

// Function declarations
void render_particles(SDL_Renderer *renderer, Particle *particles, int count);

// Collision functions
int detect_collision(Particle *p1, Particle *p2);
Particle* handle_collision(Particle *p1, Particle *p2, Particle particles[], int *count, int *capacity);

#endif

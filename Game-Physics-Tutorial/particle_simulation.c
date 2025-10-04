#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "particle_simulation.h"

//
const float damping_factor = 1;  // Slow down particles slightly


// Dynamic arrays for particles
Particle* create_particle_array(int capacity) {
    return (Particle*)malloc(capacity * sizeof(Particle));
}
Particle* resize_particle_array(Particle* particles, int* capacity) {
    *capacity *= 2; // Double the capacity
    Particle* new_particles = (Particle*)realloc(particles, *capacity * sizeof(Particle));

    if (new_particles == NULL) {
        printf("Error: Unable to allocate memory for more particles.\n");
        exit(1);
    }
    return new_particles;
}

//Prints all particles position to the output.
void PrintParticles(Particle *particles, int count) {
    for (int i = 0; i < count; ++i) {
        Particle *particle = &particles[i];
        printf("particle[%i] (%.2f, %.2f)\n", i, particle->position.x, particle->position.y);
    }
}
void PrintInformation(int count) {
    printf("%d particles\n", count);
}

// Initialize a single particle at the given (x, y) position
void init_particle_neutron(Particle *p, int index, int x, int y) {
    p[index].position = (Vector2) {(float)(x), (float)(y)};
    int direction_x = (rand() % 2 == 0) ? 1 : -1;
    int direction_y = (rand() % 2 == 0) ? 1 : -1;
    p[index].velocity = (Vector2) {VELOCITY_ONE * direction_x, VELOCITY_ONE * direction_y}; 
    p[index].radius_render = 2;
    p[index].radius = RADIUS_P; // same as the proton
    p[index].mass = MASS_P;     // same as the proton
    p[index].charge = 0;
    p[index].type = NEUTRON;
    // color
    p[index].color.r = 255;
    p[index].color.g = 255;
    p[index].color.b = 255;
    p[index].color.a = 255;  // Full opacity
    // tracking
    p[index].tab_index = index;
}
void init_particle_electron(Particle *p, int index, int x, int y) {
    p[index].position = (Vector2) {(float)(x), (float)(y)};
    int direction_x = (rand() % 2 == 0) ? 1 : -1;
    int direction_y = (rand() % 2 == 0) ? 1 : -1;
    p[index].velocity = (Vector2) {VELOCITY_ONE * direction_x, VELOCITY_ONE * direction_y}; 
    //p[index].radius_render = 1;
    p[index].radius = RADIUS_E;
    p[index].mass = MASS_E;
    p[index].charge = CHARGE_E;
    p[index].type = ELECTRON;
    // color
    p[index].color.r = 0;
    p[index].color.g = 255;
    p[index].color.b = 0;
    p[index].color.a = 255;  // Full opacity
    // tracking
    p[index].tab_index = index;

}
void init_particle_proton(Particle *p, int index, int x, int y) {
    p[index].position = (Vector2) {(float)(x), (float)(y)};
    int direction_x = (rand() % 2 == 0) ? 1 : -1;
    int direction_y = (rand() % 2 == 0) ? 1 : -1;
    p[index].velocity = (Vector2) {VELOCITY_ONE * direction_x, VELOCITY_ONE * direction_y}; 
    //p[index].radius_render = 2.5;
    p[index].radius = RADIUS_P;
    p[index].mass = MASS_P;
    p[index].charge = CHARGE_P;
    p[index].type = PROTON;
    // color
    p[index].color.r = 0;
    p[index].color.g = 255;
    p[index].color.b = 255;
    p[index].color.a = 255;  // Full opacity
    // tracking
    p[index].tab_index = index;

}

// Apply Electrostatic forces
void apply_coulomb_force(Particle *p1, Particle *p2) {
    // Calculate distance between particles
    float dx = p2->position.x - p1->position.x;
    float dy = p2->position.y - p1->position.y;
    float distance = sqrt(dx * dx + dy * dy);

    //
    if (distance < (p1->radius + p2->radius)) {
        distance = p1->radius + p2->radius; // Prevent singularities
    }

    // Apply softening to avoid division by zero when particles are very close
    float force = ke * (p1->charge * p2->charge) / (distance * distance + epsilon * epsilon) / FORCE_SCALE;       
    //float force = ke * (p1->charge * p2->charge) / (distance * distance);
    
    // Normalize the direction of the force
    float fx = (dx / distance) * fabs(force);  // Magnitude * direction
    float fy = (dy / distance) * fabs(force);  // Magnitude * direction
    
    // Determine whether to attract or repel based on the charges
    if (p1->charge * p2->charge < 0) {
        // Opposite charges -> attract: move particles toward each other
        p1->velocity.x += fx / p1->mass;
        p1->velocity.y += fy / p1->mass;

        p2->velocity.x -= fx / p2->mass;
        p2->velocity.y -= fy / p2->mass;
    } else {
        // Like charges -> repel: move particles away from each other
        p1->velocity.x -= fx / p1->mass;
        p1->velocity.y -= fy / p1->mass;

        p2->velocity.x += fx / p2->mass;
        p2->velocity.y += fy / p2->mass;
    }
}

// Apply strong nuclear force
void apply_strong_force(Particle *p1, Particle *p2) {
    float dx = p2->position.x - p1->position.x;
    float dy = p2->position.y - p1->position.y;
    float distance = sqrt(dx * dx + dy * dy);

    // Only apply strong force if within range
    if (distance <= STRONG_FORCE_RANGE) {
        //
        if (distance < (p1->radius + p2->radius)) {
            distance = p1->radius + p2->radius; // Prevent singularities
        }

        // Calculate the attractive force magnitude (arbitrary model)
        float force = STRONG_FORCE / (distance * distance + epsilon * epsilon) / FORCE_SCALE; // Inversely proportional to distance squared

        // Normalize the direction of the force
        float fx = (dx / distance) * force;
        float fy = (dy / distance) * force;

        // Apply the force to both particles (attracting them together)
        p1->velocity.x += fx / p1->mass; // Move p1 towards p2
        p1->velocity.y += fy / p1->mass;

        p2->velocity.x -= fx / p2->mass; // Move p2 towards p1
        p2->velocity.y -= fy / p2->mass;
    }
}

// find removed particles
int is_removed_particles(Particle *p, int count) {
    for(int i = 0; i < count; ++i) {
        if (p[i].type == REMOVED) return p[i].tab_index;
    }
    return -1;
}


// Update particle position
Particle* update_particles(Particle* particles, int *count, int *capacity) {
    // Apply forces between each pair of particles
    for (int i = 0; i < *count; ++i) {
        for (int j = i + 1; j < *count; ++j) {
            apply_coulomb_force(&particles[i], &particles[j]);    
            if ((particles[i].type == PROTON) && (particles[j].type == PROTON)) {
                apply_strong_force(&particles[i], &particles[j]);
            }
        }
    }


    // update particles position
    for (int i = 0; i < *count; ++i) {
        // Apply damping to the velocity
        particles[i].velocity.x *= damping_factor;
        particles[i].velocity.y *= damping_factor;
        
        particles[i].position.x += (particles[i].velocity.x * TIME_SCALE) / DISTANCE_SCALE;
        particles[i].position.y += (particles[i].velocity.y * TIME_SCALE) / DISTANCE_SCALE;

        // Bounce off the edges of the window
        if (particles[i].position.x <= 0 || particles[i].position.x >= WIDTH) particles[i].velocity.x = -particles[i].velocity.x;
        if (particles[i].position.y <= 0 || particles[i].position.y >= LENGTH) particles[i].velocity.y = -particles[i].velocity.y;
    }
    return particles; // Return updated particle array (in case it was resized)
}

// Collision functions
int detect_collision(Particle *p1, Particle *p2) {
    if (p1->type == PHOTON || p2->type == PHOTON) {
        return 0;  // Skip collision detection if either particle is a photon
    }
    float dx = p2->position.x - p1->position.x;
    float dy = p2->position.y - p1->position.y;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance < (p1->radius + p2->radius); // Collision occurs if distance is less than the sum of radii
}
Particle* handle_collision(Particle *p1, Particle *p2, Particle* particles, int *count, int *capacity) {
    // Check if we need to resize the array
    if (*count >= *capacity) {
        int r = is_removed_particles(particles, *count);
        if (r != -1) {
            particles[r].position.x = (p1->position.x + p2->position.x) / 2;
            particles[r].position.y = (p1->position.y + p2->position.y) / 2;
            particles[r].velocity = (Vector2) {VELOCITY_LIGHT, VELOCITY_LIGHT};
            particles[r].mass = 0;
            particles[r].radius = 1;
            particles[r].type = PHOTON;
            particles[r].color = (SDL_Color){255, 255, 0, 255};
            particles[r].tab_index = r;
            
            // Remove the 2 neutrons
            p1->position = (Vector2) {-1, -1};
            p1->velocity = (Vector2) {2, 2};
            p1->type = REMOVED;
            p1->color = (SDL_Color){0,0,0,0};
            p2->position = (Vector2) {-1, -1};
            p2->velocity = (Vector2) {2, 2};
            p2->type = REMOVED;
            p2->color = (SDL_Color){0,0,0,0};
            return particles;
        }
        particles = resize_particle_array(particles, capacity); // Resize the array
    }
    
    // Example: if two neutrons collide, create a proton
    if (p1->type == NEUTRON && p2->type == NEUTRON) {
        Particle new_particle;
        new_particle.position.x = (p1->position.x + p2->position.x) / 2;
        new_particle.position.y = (p1->position.y + p2->position.y) / 2;
        new_particle.velocity = (Vector2) {VELOCITY_LIGHT, VELOCITY_LIGHT};
        new_particle.mass = 0;
        new_particle.radius = 1;
        new_particle.type = PHOTON;
        new_particle.color = (SDL_Color){255, 255, 0, 255};
        new_particle.tab_index = *count;

        particles[*count] = new_particle;
        (*count)++;

        // Remove the 2 neutrons
        p1->position = (Vector2) {-1, -1};
        p1->velocity = (Vector2) {2, 2};
        p1->type = REMOVED;
        p1->color = (SDL_Color){0,0,0,0};
        p2->position = (Vector2) {-1, -1};
        p2->velocity = (Vector2) {2, 2};
        p2->type = REMOVED;
        p2->color = (SDL_Color){0,0,0,0};
    }
    return particles; // Return the updated particle array (in case it was resized)
}

// Display the particles on screen
void render_particles(SDL_Renderer *renderer, Particle *particles, int count) {
    int x, y, dx, dy, radius = 0;
    for (int i = 0; i < count; ++i) {
        SDL_SetRenderDrawColor(renderer, particles[i].color.r, particles[i].color.g, particles[i].color.b, particles[i].color.a);
        //SDL_RenderDrawPoint(renderer, (int)particles[i].position.x, (int)particles[i].position.y);
        x = (int)particles[i].position.x;
        y = (int)particles[i].position.y;
        radius = particles[i].radius;
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                dx = radius - w;
                dy = radius - h;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
}

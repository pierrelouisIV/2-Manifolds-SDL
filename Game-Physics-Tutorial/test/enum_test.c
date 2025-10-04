#include <stdio.h>


typedef enum {
    PROTON,
    NEUTRON
} ParticleType;


typedef struct {
    ParticleType type;
} Particle;


int main() {

    Particle un = {PROTON};
    Particle deux = {PROTON};

    if (un.type == PROTON && deux.type == PROTON)
        printf("Les deux sont des protons !\n");
    

    return 0;
}

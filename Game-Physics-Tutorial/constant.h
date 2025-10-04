#ifndef CONSTANT_H
#define CONSTANT_H

// Interface
#define WIDTH 900
#define LENGTH 700
#define NUM_PARTICLES 1000

//
#define VELOCITY_ONE 0
#define VELOCITY_LIGHT 10

// Constant
#define ke 8.9875e9 // Coulomb constant (N m^2/C^2)
#define epsilon 1e-10 // Small softening parameter to avoid singularity

// Electron & Proton
#define MASS_P 1.67 // 1.6726e-27 * 1e27 -> scaled mass of proton (kg)
#define MASS_E 0.0009 // 9.109e-31 * 1e27 -> scaled mass of electron (kg)
#define CHARGE_P  1.6022e-19    // Charge of proton (C) -> 1.6022e-19
#define CHARGE_E -1.6022e-19    // Charge of electro (C) -> -1.6022e-19
#define RADIUS_P 4.0   // 1.0e-15 -> Proton radius in pixels
#define RADIUS_N 4.0   // 1.0e-15 -> Neutron radius in pixels
#define RADIUS_E 1.0   // 1.0e-18 -> Electron radius in pixels


// Strong force
#define STRONG_FORCE 1e-20 // Arbitrary strong force constant (N) -> 1e10
#define STRONG_FORCE_RANGE 20 //2e1 ou 20 mais partons sur 10  // 1e-15 / 1e-10 -> scaled range of strong force (pixels)

// Scaling
#define DISTANCE_SCALE 1e-10 // 1 pixel = 1e-10 meters
#define FORCE_SCALE 1e-20   // Scaling factor for forces
#define TIME_SCALE 1e-4    // Time scaling factor for smooth motion

#endif

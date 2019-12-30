#ifndef VOICE_FUNCTIONS_H
#define VOICE_FUNCTIONS_H 1

#include <stdint.h>

#define SINE_MEAN_MAG 0.637
#define TRIANGLE_MEAN_MAG 0.5
#define SQUARE_MEAN_MAG 1

static double SineWave(double v) {
    v = sin(TAU * v);
    v *= TRIANGLE_MEAN_MAG/SINE_MEAN_MAG;
    return v;
}

static double TriangleWave(double v) {
    v -= (double)(int64_t)v;

    if (v < 0.25) {
        v = v / 0.25;
    }
    else if (v < 0.75) {
        v = 1 - 2*((v - 0.25) / 0.5);
    }
    else {          
        v = -1 + (v - 0.75) / 0.25;
    }

    return v;
}

static double SquareWave(double v) {
    v -= (double)(int64_t)v;

    if (v < 0.5) v = 1;
    else v = -1;

    v *= TRIANGLE_MEAN_MAG/SQUARE_MEAN_MAG;

    return v;
}

static double SawWave(double v) {
    v += 0.5;
    v -= (double)(int64_t)v;
    v *= 2;
    v -= 1;

    // TODO(jakob): What is the rectified mean magnitude of a saw wave? 

    return v;
}

#endif
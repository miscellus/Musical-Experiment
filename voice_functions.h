#ifndef VOICE_FUNCTIONS_H
#define VOICE_FUNCTIONS_H 1

static double triangle_wave(double v) {
    v = fmod(v, 1.0);

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

static double square_wave(double v) {
    v = fmod(v, 1.0);

    if (v < 0.5) v = 1;
    else v = -1;

    return v;
}

#endif
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "global_constants.h"
#include "wave_file.h"

static double triangle_wave(double v) {
    v = fmod(v, TAU) / TAU;

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
    v = fmod(v, TAU) / TAU;

    if (v < 0.5) v = 1;
    else v = -1;

    return v;
}

static double v[10];

typedef struct {
    double n;
    double t;
    //voice_function voice;
} play_chord;

// 120 / bpm

// 4000.0  t/m*(m/b) t/b

//120 bpm = 2.0 bps

static play_chord song2[] = {
    {00,144},
    {a4,12},
    {a4,12},
    {c5,48},
    {a4,12},
    {a4,12},
    {c5,48},
    {a4,12},
    {c5,12},
    {f5,24},
    {e5,36},
    {d5,12},
    {d5,24},
    {c5,24},
    {g4,12},
    {a4,12},
    {A4,36},
    {g4,12},
    {g4,12},
    {a4,12},
    {A4,36},
    {g4,12},
    {g4,12},
    {A4,12},
    {e5,12},
    {d5,12},
    {c5,24},
    {e5,24},
    {f5,48},
    {f4,12},
    {f4,12},
    {f5,48},
    {d5,12},
    {A4,12},
    {c5,48},
    {a4,12},
    {f4,12},
    {A4,7},
    {c5,8},
    {A4,9},
    {a4,24},
    {g4,24},
    {f4,60},
    {00,40},
};
static play_chord song[] = {
    {f4,60},
    {g4,24},
    {a4,24},
    {A4,9},
    {c5,8},
    {A4,7},
    {f4,12},
    {a4,12},
    {c5,48},
    {A4,12},
    {d5,12},
    {f5,48},
    {f4,12},
    {f4,12},
    {f5,48},
    {e5,24},
    {c5,24},
    {d5,12},
    {e5,12},
    {A4,12},
    {g4,12},
    {g4,12},
    {A4,36},
    {a4,12},
    {g4,12},
    {g4,12},
    {A4,36},
    {a4,12},
    {g4,12},
    {c5,24},
    {d5,24},
    {d5,12},
    {e5,36},
    {f5,24},
    {c5,12},
    {a4,12},
    {c5,48},
    {a4,12},
    {a4,12},
    {c5,48},
    {a4,12},
    {a4,12},
    {00,40},
};

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: music <output_filename>\n");
        return -1;
    }
    double t = 0;
    double t2 = 0;
    double fade = 1;
    int chord_index = 1;
    int chord_index2 = 0;
    double sum;
    size_t sample_count = 0;
    int16_t *samples = malloc(100*1024*1024);
    play_chord chord = song2[chord_index];
    play_chord chord2 = song2[chord_index2];

    double time_advance = 0; 
    // while (chord_index < sizeof(song)/sizeof(*song)) {
    while (sample_count < 2*SAMPLE_RATE) {

        // if (t >= chord.t) {
        //     t = fmod(t, chord.t);
        //     chord_index += 1;
        //     chord = song2[chord_index];
        //     fade = 1;
        //     v[1] = 0;
        // }
        // if (t2 >= chord2.t) {
        //     t2 = fmod(t2, chord2.t);
        //     chord_index2 += 1;
        //     chord2 = song2[chord_index2];
        //     v[2] = 0;
        // }
        //v[0] += chord.n * S12;

        time_advance += TAU / SAMPLE_RATE;
        
        // fade *= 1-4.0/SAMPLE_RATE;
        
        if (sample_count < SAMPLE_RATE) {
            sum = (
                +square_wave(time_advance * c5)
                +triangle_wave(time_advance * e5)
                +sin(time_advance * g5)
                );
        }
        else if (sample_count >= SAMPLE_RATE) {
            sum = (
                +square_wave(time_advance * c5)
                +triangle_wave(time_advance * D5)
                +sin(time_advance * g5)
                );
        }

        sum *= AMPLITUDE;

        if (sum > 1) sum = 1;
        if (sum < -1) sum = -1;

        samples[sample_count++] = (int16_t)(sum*32767.5);

        // t += 16*(2/(double)SAMPLE_RATE) * 1.3;
        // t2 += 16*(2/(double)SAMPLE_RATE) * 1.3;
    }

    save_wave_file(samples, sample_count, (char *)argv[1], SAMPLE_RATE);

    // for (double x = 0; x <= TAU; x += TAU/8) {
    //     printf("%f -> %f\n", x, triangle_wave(x));
    // }

    return 0;
}
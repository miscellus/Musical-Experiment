#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"
#include "global_constants.h"
#include "voice_functions.h"
#include "wave_file.h"

typedef struct {
    double frequency; // In hertz
    double duration; // In beats
} note_hit;

static note_hit lullaby_[] = {{00,1},
    {c3,1},{e3,1},{g3,1},
    {c4,1},{d4,1},{e4,1},{f4,1},{g4,1},{a4,1},{b4,1},
    {c5,1},{d5,1},{e5,1},{f5,1},{g5,1},{a5,1},{b5,1},
    {c6,1},{d6,1},{e6,1},{f6,1},{g6,1},{a6,1},{b6,1},
    {c7,1},{d7,1},{e7,1},{f7,1},{g7,1},{a7,1},{b7,1},
};


static note_hit lullaby[] = {
    {a4,1/2.},
    {a4,1/2.},
    {c5,2},
    {a4,1/2.},
    {a4,1/2.},
    {c5,2},
    {a4,1/2.},
    {c5,1/2.},
    {f5,1},
    {e5,3/2.},
    {d5,1/2.},
    {d5,1},
    {c5,1},
    {g4,1/2.},
    {a4,1/2.},
    {A4,3/2.},
    {g4,1/2.},
    {g4,1/2.},
    {a4,1/2.},
    {A4,3/2.},
    {g4,1/2.},
    {g4,1/2.},
    {A4,1/2.},
    {e5,1/2.},
    {d5,1/2.},
    {c5,1},
    {e5,1},
    {f5,2},
    {f4,1/2.},
    {f4,1/2.},
    {f5,2},
    {d5,1/2.},
    {A4,1/2.},
    {c5,2},
    {a4,1/2.},
    {f4,1/2.},
    {A4,7./24.},
    {c5,1./3.},
    {A4,3./8.},
    {a4,1},
    {g4,1},
    {f4,5/2.},
    {00,8},
};

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: music <output_filename>\n");
        return -1;
    }

    size_t sample_count = 0;
    double time_elapsed = 0; // in seconds
    double time_hit = 0;
    double time_hit_end = lullaby[0].duration*SECONDS_PER_BEAT;

    int note_index = 0;
    note_hit hit = lullaby[note_index];

    int16_t *samples = malloc(100*1024*1024);
    double sum;

    while (note_index < sizeof(lullaby)/sizeof(*lullaby)) {
        // Advance song if next note reached
        if (time_elapsed >= time_hit_end) {
            note_index += 1;
            hit = lullaby[note_index];
            time_hit = time_elapsed;
            time_hit_end = time_hit + hit.duration*SECONDS_PER_BEAT;
        }

        sum = triangle_wave(time_elapsed * hit.frequency);

        sum *= AMPLITUDE * (1 - pow(min(0.5,time_elapsed-time_hit)/0.5, 1.5));

        sum = clamp(sum, -1, 1);

        samples[sample_count] = (int16_t)(sum*32768);

        sample_count += 1;
        time_elapsed = sample_count * (1.0/SAMPLES_PER_SECOND);
    }

    save_wave_file(samples, sample_count, (char *)argv[1], SAMPLES_PER_SECOND);

    return 0;
}
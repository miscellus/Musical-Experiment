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
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
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

    double fade = 2*SECONDS_PER_BEAT;

    while (note_index < sizeof(lullaby)/sizeof(*lullaby)) {
        // Advance song if next note reached
        if (time_elapsed >= time_hit_end) {
            note_index += 1;
            hit = lullaby[note_index];
            time_hit = time_elapsed;
            time_hit_end = time_hit + hit.duration*SECONDS_PER_BEAT;
        }

        sum = triangle_wave(time_elapsed * hit.frequency);

        sum *= AMPLITUDE * pow((1 - min(fade, time_elapsed-time_hit)/fade),5);

        sum = clamp(sum, -1, 1);

        samples[sample_count] = (int16_t)(sum*32768);

        sample_count += 1;
        time_elapsed = sample_count * (1.0/SAMPLES_PER_SECOND);
    }

    save_wave_file(samples, sample_count, (char *)argv[1], SAMPLES_PER_SECOND);

    return 0;
}
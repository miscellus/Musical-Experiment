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
    {00,2},
};

#define MAX_SAMPLES (SAMPLES_PER_SECOND*60*60)

int main(int argc, char const *argv[])
{
    if (argc <= 2) {
        fprintf(stderr, "USAGE: music <input csv file> <output wave file>\n");
        return -1;
    }

    const char *input_song_file = argv[1];
    const char *output_wave_file = argv[2];

    loaded_song song = load_song_file(input_song_file);

    int16_t *samples = malloc(MAX_SAMPLES * sizeof(*samples));
    size_t sample_index = 0;
    double time_elapsed = 0; // in seconds
    double time_hit = 0;
    double time_hit_end = lullaby[0].duration*SECONDS_PER_BEAT;

    int note_index = 0;
    note_hit hit = lullaby[note_index];

    double sum;

    double fade = 5*SECONDS_PER_BEAT;

    fprintf(stderr, "Generating song, \"%s\":\n", argv[1]);

    while (sample_index < MAX_SAMPLES && note_index < sizeof(lullaby)/sizeof(*lullaby)) {

        // Advance song if next note reached
        if (time_elapsed >= time_hit_end) {
            note_index += 1;
            hit = lullaby[note_index];
            time_hit = time_elapsed;
            time_hit_end = time_hit + hit.duration*SECONDS_PER_BEAT;
        }

        sum = 0;
        sum += triangle_wave((time_elapsed) * hit.frequency);
        // sum += triangle_wave((time_elapsed) * hit.frequency * S12 * 0.995);
        // sum += triangle_wave((time_elapsed) * hit.frequency * S12 * 1.005);
        // sum /= 3;

        sum *= AMPLITUDE * pow((1 - min(fade, time_elapsed-time_hit)/fade),10);

        sum = clamp(sum, -1, 1);

        samples[sample_index] = (int16_t)(sum * 0x8000);

        sample_index += 1;
        time_elapsed = (double)sample_index / (double)SAMPLES_PER_SECOND;

        if (sample_index % SAMPLES_PER_SECOND == 0) {
            fprintf(stderr, "\rSong length: %d seconds.", (uint64_t)(sample_index/SAMPLES_PER_SECOND));
        }
    }
    fprintf(stderr, "\n");

    save_wave_file(samples, sample_index, output_wave_file, SAMPLES_PER_SECOND);

    return 0;
}
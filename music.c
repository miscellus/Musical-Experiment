#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"
#include "global_constants.h"
#include "voice_functions.h"
#include "files.h"

typedef struct {
    double Frequency; // In hertz
    double Duration; // In beats
} note_hit;

static note_hit Lullaby_[] = {{00,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
    {c5,1},{c5,1},{c5,1},{c5,1},{c5,1},
};


static note_hit Lullaby[] = {
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

int main(int ArgumentCount, char const *Arguments[])
{
    if (ArgumentCount <= 2) {
        fprintf(stderr, "USAGE: music <input csv file> <output wave file>\n");
        return -1;
    }

    const char *InputSongFile = Arguments[1];
    const char *OutputWaveFile = Arguments[2];

    loaded_song Song = LoadSongFile(InputSongFile);

    int16_t *Samples = malloc(MAX_SAMPLES * sizeof(*Samples));
    size_t SampleIndex = 0;
    double TimeElapsed = 0; // in seconds
    double TimeHit = 0;
    double TimeHitEnd = Lullaby[0].Duration*SECONDS_PER_BEAT;

    int NoteIndex = 0;
    note_hit Hit = Lullaby[NoteIndex];

    double Sum;

    double Fade = 5*SECONDS_PER_BEAT;

    fprintf(stderr, "Generating song, \"%s\":\n", Arguments[1]);

    while (SampleIndex < MAX_SAMPLES && NoteIndex < sizeof(Lullaby)/sizeof(*Lullaby)) {

        // Advance song if next note reached
        if (TimeElapsed >= TimeHitEnd) {
            NoteIndex += 1;
            Hit = Lullaby[NoteIndex];
            TimeHit = TimeElapsed;
            TimeHitEnd = TimeHit + Hit.Duration*SECONDS_PER_BEAT;
        }

        Sum = 0;
        Sum += TriangleWave((TimeElapsed) * Hit.Frequency);
        // sum += triangle_wave((time_elapsed) * hit.frequency * S12 * 0.995);
        // sum += triangle_wave((time_elapsed) * hit.frequency * S12 * 1.005);
        // sum /= 3;

        Sum *= AMPLITUDE * pow((1 - Min(Fade, TimeElapsed-TimeHit)/Fade),10);

        Sum = Clamp(Sum, -1, 1);

        Samples[SampleIndex] = (int16_t)(Sum * 0x8000);

        SampleIndex += 1;
        TimeElapsed = (double)SampleIndex / (double)SAMPLES_PER_SECOND;

        if (SampleIndex % SAMPLES_PER_SECOND == 0) {
            fprintf(stderr, "\rSong length: %d seconds.", (uint64_t)(SampleIndex/SAMPLES_PER_SECOND));
        }
    }
    fprintf(stderr, "\n");

    SaveWaveFile(Samples, SampleIndex, OutputWaveFile, SAMPLES_PER_SECOND);

    return 0;
}
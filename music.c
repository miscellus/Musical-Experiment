#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"
#include "global_constants.h"
#include "voice_functions.h"
#include "files.h"

#define MAX_SAMPLES (SAMPLES_PER_SECOND*60*60)

typedef struct {
    double Frequency; // In hertz
    double Duration; // In beats
} note_hit;

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

int main(int ArgumentCount, char const *Arguments[]) {
    if (ArgumentCount <= 1) {
        fprintf(stderr, "USAGE: music <csv song file>\n");
        return -1;
    }

    loaded_song Song = LoadSongFile(Arguments[1]);

    double SecondsPerBeat = 60.0 / Song.BeatsPerMinute;

    int16_t *Samples = malloc(MAX_SAMPLES * sizeof(*Samples));
    size_t SampleIndex = 0;
    double TimeElapsed = 0; // in seconds
    double TimeHit = 0;
    double TimeHitEnd = Lullaby[0].Duration*SecondsPerBeat;

    int NoteIndex = 0;
    note_hit Hit = Lullaby[NoteIndex];

    double Sum;

    double Fade = 5*SecondsPerBeat;

    fprintf(stderr, "Generating song, \"%s\":\n", Song.OutFile);

    while (SampleIndex < MAX_SAMPLES && NoteIndex < sizeof(Lullaby)/sizeof(*Lullaby)) {

        // Advance song if next note reached
        if (TimeElapsed >= TimeHitEnd) {
            NoteIndex += 1;
            Hit = Lullaby[NoteIndex];
            TimeHit = TimeElapsed;
            TimeHitEnd = TimeHit + Hit.Duration*SecondsPerBeat;
        }

        Sum = 0;
        Sum += TriangleWave(TimeElapsed * Hit.Frequency);

        Sum *= AMPLITUDE * pow((1 - Min(Fade, TimeElapsed-TimeHit)/Fade),10);

        Sum = Clamp(Sum, -1, 1);

        Samples[SampleIndex] = (int16_t)(Sum * 0x8000);

        SampleIndex += 1;
        TimeElapsed = (double)SampleIndex / (double)Song.SampleRate;

        if (SampleIndex % Song.SampleRate == 0) {
            fprintf(stderr, "\rSong length: %d seconds.", (uint32_t)(SampleIndex/Song.SampleRate));
        }
    }
    fprintf(stderr, "\n");

    SaveWaveFile(Samples, SampleIndex, Song.OutFile, Song.SampleRate);

    return 0;
}
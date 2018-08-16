#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"
#include "global_constants.h"
#include "voice_functions.h"
#include "files.h"

#define MAX_SAMPLES (SAMPLES_PER_SECOND*60*60)

#define ArraySize(A) (sizeof(A)/sizeof(*A))

#define RowToSeconds(RowTime) ((RowTime)*SecondsPerDivision)

typedef struct {
    double Frequency; // In hertz
    double Duration; // In beats
} note_hit;

static note_hit Lullaby[] = {{a4,1/2.}, {a4,1/2.}, {c5,2}, {a4,1/2.}, {a4,1/2.}, {c5,2}, {a4,1/2.}, {c5,1/2.}, {f5,1}, {e5,3/2.}, {d5,1/2.}, {d5,1}, {c5,1}, {g4,1/2.}, {a4,1/2.}, {A4,3/2.}, {g4,1/2.}, {g4,1/2.}, {a4,1/2.}, {A4,3/2.}, {g4,1/2.}, {g4,1/2.}, {A4,1/2.}, {e5,1/2.}, {d5,1/2.}, {c5,1}, {e5,1}, {f5,2}, {f4,1/2.}, {f4,1/2.}, {f5,2}, {d5,1/2.}, {A4,1/2.}, {c5,2}, {a4,1/2.}, {f4,1/2.}, {A4,7./24.}, {c5,1./3.}, {A4,3./8.}, {a4,1}, {g4,1}, {f4,5/2.}, {00,2}, };

int main(int ArgumentCount, char const *Arguments[]) {
    if (ArgumentCount <= 1) {
        fprintf(stderr, "USAGE: music <csv song file>\n");
        return -1;
    }

    loaded_song Song = LoadSongFile(Arguments[1]);

    printf( "Song = {\n"
            "    BeatsPerMinute = %d\n"
            "    SampleRate = %d\n"
            "    RowsPerBeat = %d\n"
            "    OutFile = %s\n"
            "    NumChannels = %d\n"
            "    Channels = {...}\n"
            "}\n",
            Song.BeatsPerMinute,
            Song.SampleRate,
            Song.RowsPerBeat,
            Song.OutFile,
            Song.NumChannels);

    double SecondsPerDivision = 60.0 / (Song.BeatsPerMinute * Song.RowsPerBeat);

    int16_t *Samples = malloc(MAX_SAMPLES * sizeof(*Samples));
    size_t SampleIndex = 0;
    double TimeElapsed = 0; // in seconds
    double TimeHit = 0;
    double TimeHitEnd = Lullaby[0].Duration*SecondsPerDivision;

    int NoteIndex = 0;
    note_hit Hit = Lullaby[NoteIndex];

    double Sum;

    double Fade = 5*SecondsPerDivision;

    fprintf(stderr, "Generating song, \"%s\":\n", Song.OutFile);

#if 0
    for (int ChannelID = 0; ChannelID < Song.NumChannels; ++ChannelID) {
        printf("[Channel %d]\n", ChannelID);

        song_event *Ev = Song.Channels[ChannelID].FirstEvent;
        while (Ev) {
            printf("C%d - song_event {\n"
                "    EventType = %d;\n"
                "    InstrumentID = %d;\n"
                "    RowTime = %d;\n"
                "    Frequency = %f;\n"
                "}\n",
                ChannelID,
                Ev->EventType,
                Ev->InstrumentID,
                Ev->RowTime, 
                Ev->Frequency
            );
            Ev = Ev->Next;
        } 

    }
#endif

    song_event *Ev[ArraySize(Song.Channels)];
    for (int ChannelID = 0; ChannelID < Song.NumChannels; ++ChannelID) {
        Ev[ChannelID] = Song.Channels[ChannelID].FirstEvent;
    }

    while (SampleIndex < Song.SampleRate*25) {

        Sum = 0;

        // Advance song if next note reached
        for (int ChannelID = 0; ChannelID < Song.NumChannels; ++ChannelID) {

            song_event *E = Ev[ChannelID];

            while (E->Next && TimeElapsed > RowToSeconds(E->Next->RowTime)) {
                E = E->Next;
            }

            double TimeSinceLastNote = TimeElapsed - RowToSeconds(E->RowTime);
            
            if (TimeSinceLastNote >= 0) {
                if (TimeSinceLastNote < 0) {
                    printf("TE(%f) TSLN(%f) RT(%d) SPD(%f)\n", TimeElapsed, TimeSinceLastNote, E->RowTime, SecondsPerDivision);
                }

                double TT = AMPLITUDE * pow((1 - Min(TimeSinceLastNote/Fade, 1)),2);

                Sum += TT * TriangleWave(TimeElapsed * E->Frequency);
            }
        }

        // if (TimeElapsed >= TimeHitEnd) {
        //     NoteIndex += 1;
        //     Hit = Lullaby[NoteIndex];
        //     TimeHit = TimeElapsed;
        //     TimeHitEnd = TimeHit + Hit.Duration*SecondsPerDivision;
        // }

        // Sum += TriangleWave(TimeElapsed * Hit.Frequency);

        // Sum *= AMPLITUDE * pow((1 - Min(Fade, TimeElapsed-TimeHit)/Fade),10);

        Sum = Clamp(Sum, -1, 1);

        Samples[SampleIndex++] = (int16_t)(Sum * 0x8000);

        TimeElapsed = (double)SampleIndex / (double)Song.SampleRate;

        if (SampleIndex % Song.SampleRate == 0) {
            fprintf(stderr, "\rSong length: %d seconds.", (uint32_t)(SampleIndex/Song.SampleRate));
        }
    }
    fprintf(stderr, "\n");

    SaveWaveFile(Samples, SampleIndex, Song.OutFile, Song.SampleRate);

    return 0;
}
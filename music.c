#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
// Test
#include "helpers.h"
#include "global_constants.h"
#include "voice_functions.h"
#include "files.h"

int main(int ArgumentCount, char const *Arguments[]) {

    if (ArgumentCount != 2) {
        fprintf(stderr, "USAGE: music <csv song file>\n");
        return -1;
    }

    loaded_song Song = {0};
    LoadSongFile(Arguments[1], &Song);

    fprintf(stderr, "Song = {\n"
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

    size_t MaxSamples = Song.SampleRate * 60 * 60;

    int16_t *Samples = malloc(MaxSamples * sizeof(*Samples));
    size_t SampleIndex = 0;
    double TimeElapsed = 0; // in seconds

    fprintf(stderr, "Generating song, \"%s\":\n", Song.OutFile);

    song_event *Ev[ArraySize(Song.Channels)];
    for (int Channel = 0; Channel < Song.NumChannels; ++Channel) {
        Ev[Channel] = Song.Channels[Channel].FirstEvent;
    }

    while (TimeElapsed < Song.Duration) {

        double Sum = 0;

        // Advance song if next note reached
        for (int Channel = 0; Channel < Song.NumChannels; ++Channel) {

            song_event *E = Ev[Channel];
            instrument I = InstrumentForChannel(&Song, Channel);

            while (E->Next && TimeElapsed > Song.SecondsPerRow*E->Next->Row) {
                E = E->Next;
            }

            double TimeSinceLastNote = TimeElapsed - Song.SecondsPerRow*E->Row;

            if (TimeSinceLastNote >= 0) {

                double Amplitude;
                Amplitude = 1 - Min(TimeSinceLastNote/I.Release, 1);

                Sum += Amplitude * (
                    + (
                        // + 0.33  * I.VoiceFunction(1.0005 * TimeElapsed * E->Frequency)
                        + 0.33  * I.VoiceFunction(1 * TimeElapsed * E->Frequency)
                        // + 0.33  * I.VoiceFunction(0.9995 * TimeElapsed * E->Frequency)
                    )
                    // + 0.5 * (
                        // + 1  * I.VoiceFunction(2 * TimeElapsed * E->Frequency)
                    // )
                    // + 0.1  * I.VoiceFunction(3 * TimeElapsed * E->Frequency)
                    // + 0.05 * I.VoiceFunction(4 * TimeElapsed * E->Frequency)
                );
            }
        }

        Sum *= Song.MasterVolume;
        Sum = Clamp(Sum, -1, 1);

        Samples[SampleIndex++] = (int16_t)(Sum * 0x8000);

        TimeElapsed = (double)SampleIndex / (double)Song.SampleRate;

        if (SampleIndex % Song.SampleRate == 0) {
            fprintf(stderr, "\rSong length: %d seconds.", (uint32_t)(SampleIndex/Song.SampleRate));
        }
    }

    fprintf(stderr, "\n");

    WriteWaveFile(stdout, Samples, SampleIndex, Song.SampleRate);

    return 0;
}

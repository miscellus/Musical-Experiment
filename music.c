#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define SAMPLE_RATE 44100
#include "frequencies.h"


typedef struct wav_header {
    // RIFF Header
    uint32_t riff_header; // Contains "RIFF"
    uint32_t wav_size; // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    uint32_t wave_header; // Contains "WAVE"
    
    // Format Header
    uint32_t fmt_header; // Contains "fmt " (includes trailing space)
    uint32_t fmt_chunk_size; // Should be 16 for PCM
    uint16_t audio_format; // Should be 1 for PCM. 3 for IEEE Float
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate; // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    uint16_t sample_alignment; // num_channels * Bytes Per Sample
    uint16_t bit_depth; // Number of bits per sample
    
    // Data
    uint32_t data_header; // Contains "data"
    uint32_t data_bytes; // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header;

#define four_chars(s) (uint32_t)((s[3]<<24) | (s[2]<<16) | (s[1]<<8) | s[0])

static void save_wave_file(int16_t *samples, size_t num_samples, char *filename) {
    size_t total_file_size = sizeof(wav_header) + num_samples*sizeof(int16_t);
    
    wav_header wh;
    wh.riff_header = four_chars("RIFF");
    wh.wav_size = total_file_size - 8;
    wh.wave_header = four_chars("WAVE");

    wh.fmt_header = four_chars("fmt ");
    wh.fmt_chunk_size = 16;
    wh.audio_format = 1; // 1 for PCM
    wh.num_channels = 1;
    wh.sample_rate = SAMPLE_RATE;
    wh.byte_rate = wh.sample_rate * wh.num_channels * sizeof(int16_t); //sample_rate * num_channels * Bytes Per Sample
    wh.sample_alignment = wh.num_channels * sizeof(int16_t);
    wh.bit_depth = 8*sizeof(int16_t);
    wh.data_header = four_chars("data");
    wh.data_bytes = num_samples*sizeof(int16_t);

    FILE *wf = fopen(filename, "wb");
    fwrite(&wh, sizeof(wh), 1, wf);
    fwrite(samples, sizeof(int16_t), num_samples, wf);
    fclose(wf);
}

typedef double (*voice_function)(double);

#define rerange(v, from_min, from_max, to_min, to_max) \
    ((to_min) + (((v)-(from_min))/((from_max) - (from_min))) * ((from_max) - (from_min)))

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

static play_chord song[] = {
    {a0,120},
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

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: music <output_filename>\n");
        return -1;
    }
    double t = 0;
    double fade = 1;
    int chord_index = 0;
    double sum;
    size_t sample_count = 0;
    int16_t *samples = malloc(100*1024*1024);
    play_chord chord = song[chord_index];

    double b = 0; 
    while (chord_index < sizeof(song)/sizeof(*song)) {
        if (t >= chord.t) {
            t = fmod(t, chord.t);
            chord_index += 1;
            chord = song[chord_index];
            fade = 1;
        }
        //v[0] += chord.n * S12;
        v[1] += chord.n;
        //v[2] += chord.n * s5;
        // v[3] += chord.n * S36;
        // fade *= 1-4.0/SAMPLE_RATE;
        sum = (
            // square_wave(v[0]) * (t <= 4 ? (t/4) : 1)
            +triangle_wave(v[1])
            //+square_wave(v[2])
            // +sin(v[3])
        );
        if (sum > 1) sum = 1;
        if (sum < -1) sum = -1;
        samples[sample_count++] = (int16_t)(sum*32767.5);
        t += 16*(2/(double)SAMPLE_RATE) * 1.3;
    }

    save_wave_file(samples, sample_count, (char *)argv[1]);

    for (double x = 0; x <= TAU; x += TAU/8) {
        printf("%f -> %f\n", x, triangle_wave(x));
    }

    return 0;
}
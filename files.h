#ifndef FILES_H
#define FILES_H

#include <assert.h>
#include <stdint.h>

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

static void save_wave_file(int16_t *samples, size_t num_samples, const char *filename, uint32_t sample_rate) {
    size_t total_file_size = sizeof(wav_header) + num_samples*sizeof(int16_t);
    
    wav_header wh;
    wh.riff_header = four_chars("RIFF");
    wh.wav_size = total_file_size - 8;
    wh.wave_header = four_chars("WAVE");

    wh.fmt_header = four_chars("fmt ");
    wh.fmt_chunk_size = 16;
    wh.audio_format = 1; // 1 for PCM
    wh.num_channels = 1;
    wh.sample_rate = sample_rate;
    wh.byte_rate = wh.sample_rate * wh.num_channels * sizeof(int16_t); //sample_rate * num_channels * Bytes Per Sample
    wh.sample_alignment = wh.num_channels * sizeof(int16_t);
    wh.bit_depth = 8*sizeof(int16_t);
    wh.data_header = four_chars("data");
    wh.data_bytes = num_samples*sizeof(int16_t);

    FILE *file_handle = fopen(filename, "wb");
    fwrite(&wh, sizeof(wh), 1, file_handle);
    fwrite(samples, sizeof(int16_t), num_samples, file_handle);
    fclose(file_handle);
}

typedef struct {
    char *buffer;
    size_t size;
} entire_file;

entire_file read_entire_file(const char *filename) {
    entire_file result;
    result.buffer = NULL;
    result.size = 0;

    FILE *file_handle = fopen(filename, "rb");
    fseek(file_handle, 0, SEEK_END);
    result.size = ftell(file_handle);
    rewind(file_handle);

    result.buffer = malloc(result.size);
    assert(result.buffer);

    assert(fread(result.buffer, 1, result.size, file_handle) == result.size);

    fclose(file_handle);

    return result;
}

typedef struct {
    uint32_t beats_per_minute;
    uint32_t samples_per_second;
} loaded_song;

loaded_song load_song_file(const char *filename) {
    loaded_song song;

    song.beats_per_minute = 120;
    song.samples_per_second = 44100;

    entire_file file = read_entire_file(filename);

    
}

#endif
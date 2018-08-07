#ifndef FILES_H
#define FILES_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct wav_header {
    // RIFF Header
    uint32_t RiffHeader; // Contains "RIFF"
    uint32_t WavSize; // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    uint32_t WaveHeader; // Contains "WAVE"
    
    // Format Header
    uint32_t FmtHeader; // Contains "fmt " (includes trailing space)
    uint32_t FmtChunkSize; // Should be 16 for PCM
    uint16_t AudioFormat; // Should be 1 for PCM. 3 for IEEE Float
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate; // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    uint16_t SampleAlignment; // num_channels * Bytes Per Sample
    uint16_t BitDepth; // Number of bits per sample
    
    // Data
    uint32_t DataHeader; // Contains "data"
    uint32_t DataBytes; // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header;

static void SaveWaveFile(int16_t *Samples, size_t NumSamples, const char *FileName, uint32_t SampleRate) {
    size_t TotalFileSize = sizeof(wav_header) + NumSamples*sizeof(int16_t);
    
    wav_header wh;
    wh.RiffHeader = FourChars("RIFF");
    wh.WavSize = TotalFileSize - 8;
    wh.WaveHeader = FourChars("WAVE");

    wh.FmtHeader = FourChars("fmt ");
    wh.FmtChunkSize = 16;
    wh.AudioFormat = 1; // 1 for PCM
    wh.NumChannels = 1;
    wh.SampleRate = SampleRate;
    wh.ByteRate = wh.SampleRate * wh.NumChannels * sizeof(int16_t);
    wh.SampleAlignment = wh.NumChannels * sizeof(int16_t);
    wh.BitDepth = 8*sizeof(int16_t);
    wh.DataHeader = FourChars("data");
    wh.DataBytes = NumSamples*sizeof(int16_t);

    FILE *FileHandle = fopen(FileName, "wb");
    fwrite(&wh, sizeof(wh), 1, FileHandle);
    fwrite(Samples, sizeof(int16_t), NumSamples, FileHandle);
    fclose(FileHandle);
}

typedef struct {
    char *Buffer;
    size_t Size;
} entire_file;

entire_file ReadEntireFile(const char *FileName) {
    entire_file Result;
    Result.Buffer = NULL;
    Result.Size = 0;

    FILE *FileHandle = fopen(FileName, "rb");
    fseek(FileHandle, 0, SEEK_END);
    Result.Size = ftell(FileHandle);
    rewind(FileHandle);

    Result.Buffer = malloc(Result.Size);
    assert(Result.Buffer);

    assert(fread(Result.Buffer, 1, Result.Size, FileHandle) == Result.Size);

    fclose(FileHandle);

    return Result;
}

typedef struct {
    uint32_t BeatsPerMinute;
    uint32_t SamplesPerSecond;
} loaded_song;

bool IsWhiteSpaceChar(char c) {
    return c <= ' ';
}

size_t EatWhiteSpace(char **At) {
    char *From = *At;
    
    while(IsWhiteSpaceChar(**At)) *At += 1;

    size_t CountEaten = (size_t)(*At - From);
    return CountEaten;
}

loaded_song LoadSongFile(const char *FileName) {
    loaded_song Song;

    Song.BeatsPerMinute = 120;
    Song.SamplesPerSecond = 44100;

    entire_file File = ReadEntireFile(FileName);

    return Song;
}

#endif
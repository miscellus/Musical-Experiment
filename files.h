#ifndef FILES_H
#define FILES_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>

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

void FileGetContents(const char *FilePath, char **Buffer, size_t *BufferSize) {
    
    FILE *FileHandle = fopen(FilePath, "rb");

    if (FileHandle) {
        fseek(FileHandle, 0, SEEK_END);
        size_t FileSize = ftell(FileHandle);
        rewind(FileHandle);
        
        if (FileSize > 0) {
            *Buffer = malloc(FileSize);
            assert(Buffer);
            *BufferSize = FileSize;

            size_t AmountRead = fread(*Buffer, 1, FileSize, FileHandle);

            assert(AmountRead == FileSize);
        }
        else {
            printf("Invalid file size %d for file '%s'\n", (int)FileSize, FilePath);
        }
    }
    else {
        printf("Could not read file '%s'\n", FilePath);
    }

    fclose(FileHandle);
}

typedef struct {
    uint32_t BeatsPerMinute;
    uint32_t SampleRate;
} loaded_song;

bool IsWhiteSpaceChar(char c) {
    return c <= ' ';
}

bool IsNumericChar(char c) {
    return c >= '0' && c <= '9';
}

bool IsLetterChar(char c) {
    return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z');    
}

bool IsIdentifierStartChar(char c) {
    return IsLetterChar(c) || (c == '_');
}

bool IsIdentifierChar(char c) {
    return IsIdentifierStartChar(c) || IsNumericChar(c); 
}

void GetIdentifier(char **At, char *Buffer, size_t BufferSize) {
    
    if (IsIdentifierStartChar(**At)) {
        char *From = *At;

        do *At += 1; while (IsIdentifierChar(**At));
        
        size_t Length = *At - From;

        Length = Min(Length, BufferSize - 1);

        memcpy(Buffer, From, Length);
        Buffer[Length] = '\0';
    }
    else {
        fprintf(stderr, "Invalid identifier. Identifiers must begin with a letter or an underscore.\n"); // TODO(jakob): bette error message
    }
    
}

void GetValue(char **At, char *Buffer, size_t BufferSize) {
    if (IsIdentifierChar(**At)) {
        char *From = *At;

        do *At += 1; while (IsIdentifierChar(**At));
        
        size_t Length = *At - From;

        Length = Min(Length, BufferSize - 1);

        memcpy(Buffer, From, Length);
        Buffer[Length] = '\0';
    }
}

void EatSpaces(char **At) {
    while (**At <= ' ' && **At != '\n') *At += 1;
}

// typedef char* token_type;
// #define DEF_TOKEN_TYPE(t) token_type t = #t;
// DEF_TOKEN_TYPE(Token_Unknown);
// DEF_TOKEN_TYPE(Token_MusicNote);
// DEF_TOKEN_TYPE(Token_InstrumentName);
// #undef DEF_TOKEN_TYPE

// typedef struct token {
//     token_type TokenType;
//     LengthString *String;
// } token;

// typedef struct token_node {
//     token Token;
//     struct token_node *Next;
// } token_node;


loaded_song LoadSongFile(const char *FileName) {
    loaded_song Song;

    Song.BeatsPerMinute = 120;
    Song.SampleRate = 44100;

    char *FileContents;
    size_t FileSize;

    FileGetContents(FileName, &FileContents, &FileSize);

    char *At = FileContents;

    size_t Column = 0;
    size_t Row = 0;

    while (At < (FileContents + FileSize)) {

        if (Column == 0 && *At != ',') { // Parse Settings

            char SettingBuffer[128];
            char ValueBuffer[128];

            EatSpaces(&At);
            GetIdentifier(&At, SettingBuffer, sizeof(SettingBuffer));
            EatSpaces(&At);

            if (*At != '=') {
                fprintf(stderr, "ERROR: Setting missing '=' (equals sign).\n");
            }
            ++At;
            
            EatSpaces(&At);
            GetValue(&At, ValueBuffer, sizeof(ValueBuffer));

            bool InvalidSetting = false;

            if (strcmp(SettingBuffer, "BPM") == 0) {
                Song.BeatsPerMinute = atoi(ValueBuffer);
            }
            else if (strcmp(SettingBuffer, "SampleRate") == 0) {
                Song.SampleRate = atoi(ValueBuffer);
            }
            else {
                fprintf(stderr, "ERROR: Undefined setting, '%s'.\n", SettingBuffer);
                InvalidSetting = true;
            }

            if (!InvalidSetting) {
                fprintf(stderr, "SETTING: %s=%s\n", SettingBuffer, ValueBuffer);
            }
        }

        while (At < (FileContents + FileSize)) {
            if (*At == ',') {
                ++At;
                ++Column;
                break;
            }
            else if (*At == '\n' ) {
                ++At;
                Column = 0;
                ++Row;
                break;
            }

            ++At;
        }
    }

    return Song;
}

#endif
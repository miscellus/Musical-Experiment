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
            *Buffer = malloc(FileSize + 1);
            assert(Buffer);
            *BufferSize = FileSize + 1;

            size_t AmountRead = fread(*Buffer, 1, FileSize, FileHandle);
            (*Buffer)[FileSize + 1] = '\0';

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
    uint32_t CellsPerBeat;
    char *OutFile;
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

static inline bool CharIn(char c, const char *Chars) {
    while (*Chars != c) ++Chars;
    return *Chars != '\0';
}

void EatSpaces(char **At) {
    while (**At <= ' ' && **At != '\n') *At += 1;
}

void MakeStringCanonical(char *Scan) {
    char *WriteTo = Scan;
    while (*Scan) {
        
        if (*Scan > ' ' && *Scan != '_') {
            if (*Scan >= 'A' && *Scan <= 'Z') {
                *Scan += 'a'-'A';
            }
            *WriteTo = *Scan;
            ++WriteTo;
        }
        ++Scan;
    }
    *WriteTo = '\0';
}

typedef struct {
    char Chars[128];
} cell_buffer;

size_t GetCell(char **At, cell_buffer *Buffer) {
    while (**At && ((**At <= ' ' && **At != '\n') || **At == '"')) ++*At;

    char *From = *At;
    char *LastNonSpace = *At;

    while (**At && **At != ',' && **At != '\n') {

        if (!IsWhiteSpaceChar(**At) && **At != '"') {
            LastNonSpace = *At;
        }

        ++*At;
    }

    size_t Length = LastNonSpace + 1 - From;
    Length = Min(Length, sizeof(Buffer->Chars) - 1);

    memcpy(Buffer->Chars, From, Length);

    Buffer->Chars[Length] = '\0';

    return Length;
}

#if 0
bool GetSetting(char **At, setting_buffer *SettingBuffer) {
    while ((**At <= ' ' && **At != '\n') || **At == '"') *At += 1;

    char *From = *At;
    char *LastNonSpace = *At;

    for (int Column = 0; Column < 2; ++Column) {

        char *Buffer = Column ? SettingBuffer->Value : SettingBuffer->Name;
        size_t MaxLength = Column ? sizeof(SettingBuffer->Value) : sizeof(SettingBuffer->Name);
        
        while (**At != ',' && **At != '\n' && **At != '\0') {

            if (!IsWhiteSpaceChar(**At) && **At != '"') {
                LastNonSpace = *At;
            }

            ++*At;
        }

        size_t Length = LastNonSpace + 1 - From;
        Length = Min(Length, MaxLength - 1);

        memcpy(Buffer, From, Length);

        Buffer[Length] = '\0';

        if (Column == 0 && **At != ',') {
            return false;
        }

        ++*At;
        From = *At;
    }

    return true;
}

void GetSetting(char **At, char *Buffer, size_t BufferSize) {
    
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
    char *From = *At;
    char *LastNonSpace = *At;

    while (**At != ',' && **At != '\n') {

        if (!IsWhiteSpaceChar(**At)) {
            LastNonSpace = *At;
        }

        ++*At;
    }

    size_t Length = LastNonSpace + 1 - From;

    Length = Min(Length, BufferSize - 1);

    memcpy(Buffer, From, Length);
    Buffer[Length] = '\0';

}
#endif

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

void NextCell(char **At, uint32_t *Row, uint32_t *Column) {
    switch(**At) {
    case ',':
        ++*Column;
        break;
    case '\n':
        *Column = 0;
        ++*Row;
        break;
    }
    ++*At;
}

loaded_song LoadSongFile(const char *FileName) {
    loaded_song Song;

    {
        char FileNameBuffer[1024];
        int OutFileNameLength = snprintf(FileNameBuffer, sizeof(FileNameBuffer), "%s.wav", FileName);
        assert(OutFileNameLength > 0);

        Song.OutFile = malloc(OutFileNameLength + 1);
        assert(Song.OutFile);

        memcpy(Song.OutFile, FileNameBuffer, OutFileNameLength + 1);
        Song.OutFile[OutFileNameLength] = '\0';
        
        Song.BeatsPerMinute = 120;
        Song.SampleRate = 44100;
        Song.CellsPerBeat = 4;
    }
    
    char *FileContents;
    size_t FileSize;

    FileGetContents(FileName, &FileContents, &FileSize);

    char *At = FileContents;

    uint32_t Column = 0;
    uint32_t Row = 0;

    cell_buffer Setting = {0};
    cell_buffer Value = {0};
    cell_buffer Scratch = {0};

    while (At < (FileContents + FileSize)) {

        if (Column == 0 && *At != ',') { // Parse Settings

            if (GetCell(&At, &Setting) == 0) {
                NextCell(&At, &Row, &Column);
                continue;
            }
            NextCell(&At, &Row, &Column);

            if (Column != 1) fprintf(stderr, "ERROR(Row %u, Col %u): Setting, %s, missing value.\n", Row, Column, Setting.Chars);
            else GetCell(&At, &Value);
            
            MakeStringCanonical(Setting.Chars);

            bool UnknownSetting = false;

            if (strcmp(Setting.Chars, "bpm") == 0) {
                Song.BeatsPerMinute = atoi(Value.Chars);
            }
            else if (strcmp(Setting.Chars, "sample""rate") == 0) {
                Song.SampleRate = atoi(Value.Chars);
            }
            else if (strcmp(Setting.Chars, "cells""per""beat") == 0) {
                Song.CellsPerBeat = atoi(Value.Chars);
            }
            else if (strcmp(Setting.Chars, "output""file") == 0) {
                size_t Length = strlen(Value.Chars) + 1;
                Song.OutFile = malloc(Length);
                assert(Song.OutFile);
                memcpy(Song.OutFile, Value.Chars, Length);
            }
            else {
                fprintf(stderr, "WARNING: Unknown setting, '%s'.\n", Setting.Chars);
                UnknownSetting = true;
            }

            if (!UnknownSetting) {
                fprintf(stderr, "SETTING: %s = %s\n", Setting.Chars, Value.Chars);
            }
        }

        fprintf(stderr, "(R%d, C%d)\n", Row, Column);

        while (At < (FileContents + FileSize)) {
            //GetCell(&At, &Scratch);
            //printf("[%s]\n", Scratch.Chars);
            //NextCell(&At, &Row, &Column);
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

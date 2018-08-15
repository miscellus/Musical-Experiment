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
        *BufferSize = ftell(FileHandle);
        rewind(FileHandle);
        
        if (*BufferSize > 0) {
            *Buffer = malloc(*BufferSize + 1);
            assert(*Buffer);

            size_t AmountRead = fread(*Buffer, 1, *BufferSize, FileHandle);
            (*Buffer)[*BufferSize] = '\0';

            assert(AmountRead == *BufferSize);
        }
        else {
            printf("Invalid file size %d for file '%s'\n", (int)*BufferSize, FilePath);
        }
    }
    else {
        printf("Could not read file '%s'\n", FilePath);
    }

    fclose(FileHandle);
}

typedef enum {
    EventType_Null = 0,
    EventType_Hit = 1,
    EventType_Release = 2,
} song_event_type;

typedef struct song_event{
    song_event_type EventType:16;
    uint16_t InstrumentID;
    uint16_t RowTime;
    double Frequency;
    struct song_event *Next;
} song_event;

typedef struct {
    song_event *FirstEvent;
    song_event *LastEvent;
} song_channel;

typedef struct {
    uint32_t BeatsPerMinute;
    uint32_t SampleRate;
    uint32_t CellsPerBeat;
    char *OutFile;
    uint32_t NumChannels;
    song_channel Channels[16];
} loaded_song;

bool IsWhiteSpaceChar(char c) {
    return c <= ' ';
}

bool IsNumericChar(char c) {
    return c >= '0' && c <= '9';
}

static inline bool IsLetterChar(char c) {
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

static inline char LowerCase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 'a'-'A';
    }
    return c;
}

void EatSpaces(char **At) {
    while (**At <= ' ' && **At != '\n') *At += 1;
}

bool CompareCanonical(char *Src, const char *Dst) {
    while (*Src && *Dst) {
        if (IsWhiteSpaceChar(*Src) || *Src == '_' || *Src == '-') {
            ++Src;
        }
        else if (LowerCase(*Src) != LowerCase(*Dst)) {
            return false;
        }
        else {
            ++Src;
            ++Dst;
        }
    }

    return *Src == '\0' && *Dst == '\0';
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
    while (**At && ((**At <= ' ' && **At != '\n') || **At == '"')) {
        ++*At;
    }

    char *From = *At;
    char *LastNonSpace = *At;

    while (**At && **At != ',' && **At != '\n') {

        if (!IsWhiteSpaceChar(**At) && **At != '"') {
            LastNonSpace = *At + 1;
        }

        ++*At;
    }

    size_t Length = LastNonSpace - From;
    Length = Min(Length, sizeof(Buffer->Chars) - 1);

    memcpy(Buffer->Chars, From, Length);

    Buffer->Chars[Length] = '\0';

    return Length;
}

void NextCell(char **At, uint32_t *Row, uint32_t *Column) {
    while (**At && **At != ',' && **At != '\n') {
        ++*At;
    }

    if (**At == ',') {
        ++*Column;
        ++*At;
    }
    else if (**At == '\n') {
        *Column = 0;
        ++*Row;
        ++*At;
    }
}

loaded_song LoadSongFile(const char *FileName) {

    loaded_song Song = {0};

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
        Song.CellsPerBeat = 1;
    }
    
    char *FileContents;
    size_t FileSize;

    FileGetContents(FileName, &FileContents, &FileSize);

    char *At = FileContents;

    uint32_t Column = 0;
    uint32_t Row = 0;

    uint8_t ChannelToInstrument[256] = {0};

    cell_buffer Setting = {0};
    cell_buffer Value = {0};

    while (At < (FileContents + FileSize)) {

        /******************/
        /* Parse Settings */
        /******************/
        if (Column == 0) {

            size_t SettingLength = GetCell(&At, &Setting);
            NextCell(&At, &Row, &Column);

            if (SettingLength == 0) {
                NextCell(&At, &Row, &Column); // Skip Column 1
                continue;
            }

            size_t ValueLength = GetCell(&At, &Value);
            
            // MakeStringCanonical(Setting.Chars);

            bool UnknownSetting = false;

            if (CompareCanonical(Setting.Chars, "bpm")) {
                Song.BeatsPerMinute = atoi(Value.Chars);
            }
            else if (CompareCanonical(Setting.Chars, "sample""rate")) {
                Song.SampleRate = atoi(Value.Chars);
            }
            else if (CompareCanonical(Setting.Chars, "cells""per""beat")) {
                Song.CellsPerBeat = atoi(Value.Chars);
            }
            else if (CompareCanonical(Setting.Chars, "instrument")) {
                // TODO
            }
            else if (CompareCanonical(Setting.Chars, "output""file")) {
                size_t Length = ValueLength + 1;
                Song.OutFile = malloc(Length);
                assert(Song.OutFile);
                memcpy(Song.OutFile, Value.Chars, Length);
            }
            else {
                fprintf(stderr, "[  ] %s = %s;\n", Setting.Chars, Value.Chars);
                UnknownSetting = true;
            }

            if (!UnknownSetting) {
                fprintf(stderr, "[OK] %s = %s;\n", Setting.Chars, Value.Chars);
            }
        }
        /***************/
        /* Parse Notes */
        /***************/
        else if (Row > 0) {

            cell_buffer Cell;
            song_event *Ev = malloc(sizeof(*Ev));
            *Ev = (song_event){0};

            if (GetCell(&At, &Cell) > 0) {
                size_t Channel = Column - 2;
                assert(Channel < 16);

                Ev->EventType = EventType_Hit;
                Ev->InstrumentID = ChannelToInstrument[Channel];
                Ev->RowTime = Row - 1;
                Ev->Frequency = NoteStringToFrequency(Cell.Chars);

                if (NULL == Song.Channels[Channel].FirstEvent) {
                    Song.Channels[Channel].FirstEvent = Ev;
                    Song.Channels[Channel].LastEvent = Ev;
                }
                else {
                    Song.Channels[Channel].LastEvent->Next = Ev;
                    Song.Channels[Channel].LastEvent = Ev;
                }
            }

            NextCell(&At, &Row, &Column);
        }
        /************************/
        /* Parse Instrument IDs */
        /************************/
        else if (Row == 0) {
            cell_buffer InstrumentID;

            if (GetCell(&At, &InstrumentID) > 0) {
                size_t Channel = Column - 2;
                ChannelToInstrument[Channel] = atoi(InstrumentID.Chars);
                ++Song.NumChannels;
            }
            NextCell(&At, &Row, &Column);
        }
        else {
            assert(false);
        }
    }
    
    return Song;
}

#endif
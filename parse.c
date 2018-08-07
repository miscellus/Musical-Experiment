#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "length_strings.h"

typedef char* TokenType;

#define DEF_TOKEN_TYPE(t) TokenType t = #t;
DEF_TOKEN_TYPE(Token_Unknown);
DEF_TOKEN_TYPE(Token_Identifier);
DEF_TOKEN_TYPE(Token_IntLiteral);
DEF_TOKEN_TYPE(Token_FloatLiteral);
DEF_TOKEN_TYPE(Token_ParenOpen);
DEF_TOKEN_TYPE(Token_ParenClose);
#undef DEF_TOKEN_TYPE

typedef struct
{
    TokenType kind;
    LengthString *string;
} Token;

typedef struct TokenNode
{
    Token token;
    struct TokenNode *next;
} TokenNode;

LengthString *fileGetContents(char *filePath)
{
    LengthString *result = 0;
    
    FILE *fileHandle = fopen(filePath, "rb");

    if (fileHandle)
    {
        fseek(fileHandle, 0, SEEK_END);
        long fileSize = ftell(fileHandle);
        rewind(fileHandle);
        
        if (fileSize > 0)
        {
            result = lsMakeString(fileSize);
            fread(result->chars, fileSize, 1, fileHandle);
            fclose(fileHandle);
        }
        else
        {
            printf("Invalid file size %d for file '%s'\n", (int)fileSize, filePath);
        }
    }
    else
    {
        printf("Could not read file '%s'\n", filePath);
    }

    return result;
}

bool isWhiteSpaceChar(char c)
{
    return c <= ' ';
}

bool isNumericChar(char c)
{
    return c >= '0' && c <= '9';
}

bool isLetterChar(char c)
{
    return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z');    
}

bool isIdentifierStartChar(char c)
{
    return isLetterChar(c) || (c == '_');
}

bool isIdentifierChar(char c)
{
    return isIdentifierStartChar(c) || isNumericChar(c); 
}

void eatWhiteSpace(char **at)
{
    char *from = *at;
    
    while(isWhiteSpaceChar(**at)) *at += 1;

    int numWhiteSpaceChars = (int)(*at - from);
    return numWhiteSpaceChars;
}

Token getToWhiteSpace(char **at)
{
    char buffer[1024]; // @Hardcode
    int length = 0;

    char *bufferAt = buffer;

    while(!isWhiteSpaceChar(**at) && !isNumericChar(**at) && !isIdentifierChar(**at))
    {
        *bufferAt++ = *(*at)++;
        ++length;
    }

    LengthString *string = lsMakeString(length);
    lsAssignFromCharArray(string, buffer, length);
    Token result = (Token){Token_Unknown, string};

    return result;
}

Token makeTokenFromBuffer(TokenType tokenType, char *buffer, int length)
{
    LengthString *string = lsMakeString(length);
    lsAssignFromCharArray(string, buffer, length);
    return (Token) {tokenType, string};
}

typedef enum {
    Parse_TopLevel,
    Parse_Expression
} ParseState;

int main(int numArguments, char *arguments[])
{
    LengthString *script = fileGetContents(arguments[1]);

    char *at = script->chars;

    Token tokens[512]; //@Hardcode
    Token *currentToken = tokens;

    ParseState parseState = Parse_TopLevel;

    bool parsing = true;
    while (parsing && at < (script->chars + script->length))
    {
        switch(parseState)
        {
            case Parse_TopLevel:
            {
                eatWhiteSpace(&at);
                char buffer[1024]; // @Hardcode
                char *bufferAt = buffer;
                
                if (isIdentifierStartChar(*at))
                {
                    do
                    {
                        *bufferAt++ = *at++;
                    } while (isIdentifierChar(*at));
                    *currentToken++ = makeTokenFromBuffer(Token_Identifier, buffer, (int)(bufferAt-buffer));
                }
                else if (isNumericChar(*at))
                {
                    // Parse number
                    TokenType tokenType;
                    int dotsFound = 0;
                    do
                    {
                        *bufferAt++ = *at++;
                        if (*at == '.') ++dotsFound;
                    } while (isNumericChar(*at) || *at == '.');
                    
                    switch (dotsFound)
                    {
                        case 0: tokenType = Token_IntLiteral; break;
                        case 1: tokenType = Token_FloatLiteral; break;
                        default:
                        {
                            tokenType = Token_Unknown;
                            printf("ERROR: Trying to parse float literal, but found %d decimal dots.\n", dotsFound);
                        }
                    }
                    *currentToken++ = makeTokenFromBuffer(tokenType, buffer, (int)(bufferAt-buffer));
                }
                else
                {
                    ++at;
                    //TODO more tokens
                }
            } break;
        }
    }
    for (int i = 0, l = (currentToken - tokens); i < l; ++i)
    {
        Token token = tokens[i];
        printf("%12.*s :: %s\n", LS_SPREAD(token.string->), token.kind);
    }
    printf("\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 4096

int is_squashable(char c) {
    return c == '+' || c == '-' || c == '>' || c == '<';
}

int get_squashed(char c) {
    switch (c) {
        case '+': return 'p';
        case '-': return 'm';
        case '>': return 'f';
        case '<': return 'b';
        default: return 0;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <in> <out>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Couldn't open brainfuck source file");
        return -1;
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* contents = malloc(size + 2);
    contents[0] = ' ';
    contents[size] = 0;
    size_t i = 0;

    while (!feof(file))
        fread(contents + i++ * CHUNK_SIZE + 1, 1, CHUNK_SIZE, file);
    
    fclose(file);

    char* ir = malloc(size + 1);
    char* currChar = contents;
    size_t line = 1;
    size_t col = 1;
    int count = 0;
    char* ir_p = ir;
    char lastChar = ' ';

    for (i = 1; i < size + 1; i++) {
        char thisChar = contents[i];

        col++;

        switch (thisChar) {
        case '[':
        case ']':
        case '.':
        case ',':
            if (is_squashable(lastChar)) {
                if (count == 1) {
                    *(ir_p++) = lastChar;
                    count = 0;
                } else if (count > 0) {
                    *(ir_p++) = get_squashed(lastChar);
                    *(ir_p++) = count + '0';
                    count = 0;
                }
            }

            *(ir_p++) = thisChar;
            lastChar = thisChar;
            break;
        case '+':
        case '-':
        case '>':
        case '<':
            if (lastChar != thisChar) {
                if (count == 1) {
                    *(ir_p++) = lastChar;
                    count = 0;
                } else if (count > 0) {
                    *(ir_p++) = get_squashed(lastChar);
                    *(ir_p++) = count + '0';
                    count = 0;
                }
            }

            lastChar = thisChar;
            count++;
        case '\r':
            col = 1;
            line++;
            break;
        case '\n':
            if (*(contents - 1) == '\r') break; // Handle windows line breaks
            col = 1;
            line++;
            break;
        }
    }
    
    if (is_squashable(lastChar)) {
        if (count == 1) {
            *(ir_p++) = lastChar;
        } else if (count > 0) {
            *(ir_p++) = get_squashed(lastChar);
            *(ir_p++) = count + '0';
        }
    }

    size_t ir_size = ir_p - ir;
    FILE* outFile = fopen(argv[2], "w");
    fwrite(ir, 1, ir_size, outFile);
    fclose(outFile);

    return 0;
}
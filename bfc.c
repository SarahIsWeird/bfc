#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 4096

#define HEADER "section .data\nmem: times 10000 db 0\nsection .text\nglobal _start\n_start:\nlea rbx, [mem]\nmov rdx,1\n"
#define HEADER_LEN 97

#define FOOTER "xor rdi,rdi\nmov rax,60\nsyscall\n"
#define FOOTER_LEN 31

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
    int openLoops = 0;

    for (i = 1; i < size + 1; i++) {
        char thisChar = contents[i];

        col++;

        if (thisChar == '[') openLoops++;
        if (thisChar == ']') openLoops--;

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
                    *(ir_p++) = count;
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
                    *(ir_p++) = count;
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
            if (*(contents - 1) == '\r') break; /* Handle windows line breaks */
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
            *(ir_p++) = count;
        }
    }

    if (openLoops > 0) {
        fprintf(stderr, "%d loops were never closed.", openLoops);
        free(contents);
        free(ir);
        return -2;
    }

    if (openLoops < 0) {
        fprintf(stderr, "%d loops were closed but never opened.", openLoops);
        free(contents);
        free(ir);
        return -3;
    }

    size_t ir_size = ir_p - ir;
    FILE* outFile = fopen(argv[2], "w");
    size_t *stack = malloc(sizeof(size_t) * 10000);
    size_t *stack_ptr = stack;
    size_t loop_n = 0;
    
    fwrite(HEADER, 1, HEADER_LEN, outFile);

    for (i = 0; i < ir_size; i++) {
        switch (ir[i]) {
        case '+':
            fwrite("inc byte [rbx]\n", 1, 15, outFile);
            break;
        case '-':
            fwrite("dec byte [rbx]\n", 1, 15, outFile);
            break;
        case '>':
            fwrite("inc rbx\n", 1, 8, outFile);
            break;
        case '<':
            fwrite("dec rbx\n", 1, 8, outFile);
            break;
        case 'p':
            fprintf(outFile, "add byte [rbx],%d\n", ir[++i]);
            break;
        case 'm':
            fprintf(outFile, "sub byte [rbx],%d\n", ir[++i]);
            break;
        case 'f':
            fprintf(outFile, "add rbx,%d\n", ir[++i]);
            break;
        case 'b':
            fprintf(outFile, "sub rbx,%d\n", ir[++i]);
            break;
        case '[':
            *(stack_ptr++) = loop_n;
            fprintf(outFile, ".l%ld:cmp byte [rbx], 0\nje .l%ldd\n", loop_n, loop_n);
            loop_n++;
            break;
        case ']':
            stack_ptr--;
            fprintf(outFile, "jmp .l%ld\n.l%ldd:", *stack_ptr, *stack_ptr);
            break;
        case '.':
            fwrite("mov rsi,rbx\nmov rdi,1\nmov rax,1\nsyscall\n", 1, 40, outFile);
            break;
        case ',':
            fwrite("mov rsi,rbx\nmov rdi,0\nmov rax,0\nsyscall\n", 1, 40, outFile);
            break;
        }
    }


    fwrite(FOOTER, 1, FOOTER_LEN, outFile);

    fclose(outFile);

    free(contents);
    free(ir);
    free(stack);

    return 0;
}
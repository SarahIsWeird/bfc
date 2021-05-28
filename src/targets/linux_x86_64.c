#include "linux_x86_64.h"

#define HEADER "section .data\nmem: times 10000 db 0\nsection .text\nglobal _start\n_start:\nlea rbx, [mem]\nmov rdx,1\n"
#define HEADER_LEN 97

#define FOOTER "xor rdi,rdi\nmov rax,60\nsyscall\n"
#define FOOTER_LEN 31

static const char *_ir;
static size_t _ir_size;
static size_t *_stack;
static size_t *_stack_ptr;
static size_t loop_n = 0;

void linux_x86_64_init(const char *ir, size_t ir_size) {
    _ir = ir;
    _ir_size = ir_size;
    _stack = malloc(sizeof(size_t) * 10000);
    _stack_ptr = _stack;
}

void linux_x86_64_write(FILE *file) {
    size_t i;

    fwrite(HEADER, 1, HEADER_LEN, file);

    for (i = 0; i < _ir_size; i++) {
        switch (_ir[i]) {
        case '+':
            fwrite("inc byte [rbx]\n", 1, 15, file);
            break;
        case '-':
            fwrite("dec byte [rbx]\n", 1, 15, file);
            break;
        case '>':
            fwrite("inc rbx\n", 1, 8, file);
            break;
        case '<':
            fwrite("dec rbx\n", 1, 8, file);
            break;
        case 'p':
            fprintf(file, "add byte [rbx],%d\n", _ir[++i]);
            break;
        case 'm':
            fprintf(file, "sub byte [rbx],%d\n", _ir[++i]);
            break;
        case 'f':
            fprintf(file, "add rbx,%d\n", _ir[++i]);
            break;
        case 'b':
            fprintf(file, "sub rbx,%d\n", _ir[++i]);
            break;
        case '[':
            *(_stack_ptr++) = loop_n;
            fprintf(file, ".l%ld:cmp byte [rbx], 0\nje .l%ldd\n", loop_n, loop_n);
            loop_n++;
            break;
        case ']':
            _stack_ptr--;
            fprintf(file, "jmp .l%ld\n.l%ldd:", *_stack_ptr, *_stack_ptr);
            break;
        case '.':
            fwrite("mov rsi,rbx\nmov rdi,1\nmov rax,1\nsyscall\n", 1, 40, file);
            break;
        case ',':
            fwrite("mov rsi,rbx\nmov rdi,0\nmov rax,0\nsyscall\n", 1, 40, file);
            break;
        case 'o':
            fwrite("mov rsi,rbx\nsyscall\n", 1, 20, file);
            break;
        case 'i':
            fwrite("mov rsi,rbx\nsyscall\n", 1, 20, file);
            break;
        }
    }

    fwrite(FOOTER, 1, FOOTER_LEN, file);

    free(_stack);
}
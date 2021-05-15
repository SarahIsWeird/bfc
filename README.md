# bfc 🤯

bfc is a Brainfuck compiler. Currently, it doesn't directly compile to an executable format, but rather to Linux x86_64 NASM. Directly compiling to a binary format and other targets is planned for the near future.

## Building

bfc is super easy to build and has no external dependencies, other than the standard C library.

```bash
git clone https://github.com/SarahIsWeird/bfc
cd bfc
gcc -o bfc bfc.c
```

## Usage

Right now, there isn't anything you can configure. You run the program with `bfc <input> <output>`. After bfc ran through, you need to compile the resulting NASM file and link it.

```bash
./bfc myProgram.bf myProgram.asm
nasm -f elf64 myProgram.asm
ld -o myProgram myProgram.o
```

Congratulations! You've just compiled brainfuck code!

## Example program

There's an example program included at example.bf, if you want to try out the compiler, but don't have any brainfuck code ready. It prints out a well-known text!
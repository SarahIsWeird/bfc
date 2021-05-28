# bfc 🤯

bfc is a Brainfuck compiler. Currently, it doesn't directly compile to an executable format, but rather to Linux x86_64 NASM. Directly compiling to a binary format and other targets is planned for the near future.

## Building

bfc is super easy to build and has no external dependencies, other than the standard C library.

```bash
git clone https://github.com/SarahIsWeird/bfc
cd bfc
make
```

## Usage

Right now, there isn't anything you can configure. You run the program with `bfc <input> <output>`. After bfc ran through, you need to compile the resulting NASM file and link it. Congratulations! You've just compiled brainfuck code!

### Linux

```bash
./bfc myProgram.bf myProgram.asm
nasm -f elf64 myProgram.asm
ld -o myProgram myProgram.o
```

### macOS

Make sure you have the command line developer tools installed!

```bash
./bfc myProgram.bf myProgram.asm
nasm -f macho64 myProgram.asm
ld -macosx_version_min 11.3.1 -o myProgram myProgram.o -L$(xcode-select -p)/SDKs/MacOSX.sdk/usr/lib -lSystem
```

Why all the extra stuff when linking? Well... macOS requires creating a binary for a minimum macOS version, as well as linking to libSystem. `$(xcode-select -p)` just fetches the appropriate path where libSystem lies.

## Example program

There's an example program included at example.bf, if you want to try out the compiler, but don't have any brainfuck code ready. It prints out a well-known text!
export TARGET=test

nasm -f macho64 -o $TARGET.o $TARGET.nasm -w-zeroing
ld $TARGET.o -o $TARGET -macosx_version_min 10.13 \
    -L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib \
    -lSystem
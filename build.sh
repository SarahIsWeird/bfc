clang -o bfc bfc.c -std=c89
./bfc test.bf out.bf
hexdump -C out.bf
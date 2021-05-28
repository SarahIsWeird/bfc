#if !defined(BFC_TARGET_MAC_64_H)
#define BFC_TARGET_MAC_64_H

#include <stdio.h>
#include <stdlib.h>

void mac_64_init(const char *ir, size_t ir_size);
void mac_64_write(FILE *file);

#endif // BFC_TARGET_MAC_64_H

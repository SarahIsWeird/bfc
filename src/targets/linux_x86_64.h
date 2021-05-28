#if !defined(BFC_TARGET_LINUX_X86_64)
#define BFC_TARGET_LINUX_X86_64

#include <stdio.h>
#include <stdlib.h>

void linux_x86_64_init(const char *ir, size_t ir_size);
void linux_x86_64_write(FILE *file);

#endif /* BFC_TARGET_LINUX_X86_64 */

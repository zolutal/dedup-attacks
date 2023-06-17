#include <x86intrin.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// write a null byte to addr
void poke(char *addr) { *addr = '\0'; }

// return the difference in the processor's timestamp before and after poke
uint64_t time_poke(char *addr) {
    uint64_t start = __rdtsc();
    poke(addr);
    uint64_t end = __rdtsc();
    return end-start;
}

// allocate a single read/write anon/private page
void *alloc_page() {
    return mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
}

int main() {
    void *page = alloc_page();

    // demonstrates that faulting accesses have distinct timings
    printf("fault     : %ld cycles\n", time_poke(page));
    printf("post-fault: %ld cycles\n", time_poke(page));
}

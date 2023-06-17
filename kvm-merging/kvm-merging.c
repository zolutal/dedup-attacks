#include <x86intrin.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// write a byte to addr
void poke(char *addr) { *addr = 0x41; }

// return the difference in the processor's timestamp before and after poke
uint64_t time_poke(char *addr) {
    uint64_t start = __rdtsc();
    poke(addr);
    uint64_t end = __rdtsc();
    return end-start;
}

// read a byte from addr
void maccess(char *addr) { volatile char c = *addr; }

// time maccess using the processor timestamp
uint64_t time_access(char *addr) {
    uint64_t start = __rdtsc();
    maccess(addr);
    uint64_t end = __rdtsc();
    return end-start;
}

// allocate a single read/write anon/private page
void *alloc_page() {
    return mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
}

int main() {
    // allocate victim and attacker pages
    void *victim = alloc_page();
    void *attacker = alloc_page();

    // write something unique to both pages so they aren't merged with
    // other zero pages, this also faults them to be sure they are allocated
    memset((char *)victim, 0x41, 0x1000);
    memset((char *)attacker, 0x41, 0x1000);

    while (1) {
        printf("sleeping to wait for merge...\n");

        sleep(20);

        printf("finished sleeping... checking access times\n");

        // make sure attacker is present and in cache
        time_access(attacker);

        printf("write : %ld cycles\n", time_poke(attacker));
        printf("write : %ld cycles\n", time_poke(attacker));
    }

    return 0;
}

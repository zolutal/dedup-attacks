# Usage: boot kernel in qemu using the -s debug flag
# attach with gdb and source this script
# send the base address of the kernel when prompted

import gdb

base = int(input("kernel base in hex? "), 16)
slide = (base - 0xffffffff80000000) >> 21
print("slide:", hex(slide))

inferior = gdb.selected_inferior()

output = "uint64_t entries[256] = { " #}
for off in range(0, 0x1000, 0x10):
    entry = inferior.read_memory(0xfffffe0000000000 + off, 0x8).tobytes()
    entry = int.from_bytes(entry, 'little') - (slide << 53)
    output += hex(entry) + ', '
output = output[:-2]
output += ' };'

print(output)

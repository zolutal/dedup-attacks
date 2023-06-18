Files associated with my [blog post](https://zolutal.github.io/dedup-attacks) on memory deduplication attacks.

fault-timing: demonstrates timing differences between faulting and normal memory accesses

madvise-mergeable: demonstrates timing differences when a page marked as mergeable using madvise MADV_MERGEABLE has been merged

kvm-merging: demonstrates timing difference when a page in KVM is merged by Kernel Samepage Merging

kvm-kaslr: attack code demonstrating the ability to break KASLR inside a VM and across VMs using Kernel Samepage Merging, also includes a python script for extracting and reformatting IDT entries for the attack

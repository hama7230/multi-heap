# Multi Heap
from TokyoWesterns CTF 2019

## 配布物
* multi\_heap
* libc.so.6

## intended solution
my intended solution use an exploitation technique as known as  _wild copy_.
_wild copy_ was proposed by Google Project Zero.
original blog post is [here](https://googleprojectzero.blogspot.com/2015/03/taming-wild-copy-parallel-thread.html).

you can input negative integer as a size for copy function because of no checking for it.
a memory copy that the copy size is negative become huge heap overflow and cause a segmentation fault.
if you can use some data on memory before causing the segmentation fault, this heap overflow can be used for exploit.
this challenge was written by C++, so there is a pointer of vtable on heap segment.
you can overwrite the pointer by _wild copy_ then control rip.


Of course, I heard that unintended solution exists.
I didn't notice that, so some teams who found and used it are awesome and smart.

```
% python exploit.py r
[*] '/Users/hama/ctf/making/multi-heap/multi_heap'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to multiheap.chal.ctf.westerns.tokyo on port 10001: Done
[*] Pwning
[*] libc_base: 0x7f5700fd3000
[*] heap_base: 0x55cd53c44000
[*] Switching to interactive mode
: $ ls
flag
multi_heap
$ cat flag
TWCTF{mulmulmulmultititi}
```


## md5
```
# md5sum ./multi_heap                                                                                                    (git)-[master]
6066b86031b6f286642c231b504c4d1c  ./multi_heap
```

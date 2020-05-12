# Introduction

For a long time I wanted to understand, not only to know, how an
operating system works. But I was not able to find a path for reaching
that goal.

During the previous years I gained a lot of theoretical and
higher-level knowledge about how operating systems work. However, when
diving into a source code of a real operating system, it always seemed
to me like a different world. I also read some operating system
programming tutorials. It was a bit better but still far away from
what I was looking for. The information there was much more practical,
but it was explained as a final result of the author's learning. What
I needed the most was a story; a process of learning.

That's the reason why I decided to start writing this diary. Well,
it's not going to be a diary in the exact sense of the word but I
haven't found a better one. I wanted something personal, capturing a
story of creating a simple operatign system, and to which I would be
adding lines bit by bit at least every few days. Something real what
would show a way of thinking and dead ends as I'm starting from
scratch without any significat previous practical experience in an OS
internals.

If you find grammar mistakes, broken links, or similar errors or you
would just like to drop me a line, please write me an
[e-mail](https://github.com/jansucan), create an issue, or open a pull
request [here](https://github.com/jansucan/beginner-os-developer-diary).

# What I already know

These are my most important sources of information about OS so far

- My studies at
  [Faculty of Information Technology, Czech Technical University in Prague](https://fit.cvut.cz/)
- [brokenthorn: OS development series](http://www.brokenthorn.com/Resources/OSDevIndex.html)

# Goals

- Gain experience with and understanding of Intel x86 platform
    - protected mode
	- segmentation
	- paging
	- task switching
	- symmetric multiprocessing
- Get from Assembly a to higher-level language
    - write the kernel in C or embedded C++

I will try to give this work at least 30 minutes a day.

# Development environment

I am working in a GNU/Linux environment.

Advantages of using a virtual machine are obvious. I will try to use
Qemu because I already have it installed. It should also support
debugging ([wiki osdev: Qemu](https://wiki.osdev.org/QEMU)).

# Bootloader

The first step is to get my code loaded by the Qemu's BIOS. I'm going to read

- [wiki osdev: Boot sequence](https://wiki.osdev.org/Boot_Sequence)
- [wiki osdev: Rolling your own bootloader](https://wiki.osdev.org/Rolling_Your_Own_Bootloader)

I would like to boot my OS from a USB drive image instead of a floppy
disk. I'm going to write a simple boot code which will be placed in
MBR in the image. The code will print a message and halt the CPU.

- [root: Simple bootsector](https://www.root.cz/clanky/piseme-operacni-system-boot/) (In Czech language)
- [linux kvm: Booting from USB image](https://www.linux-kvm.org/page/USB#Running)
- [stackoverflow: Qemu boot from raw image](https://stackoverflow.com/questions/47235461/how-to-resolve-specify-the-raw-format-explicitly-to-remove-the-restrictions)
- [qemu weilnetz: Qemu documentation](https://qemu.weilnetz.de/doc/qemu-doc.html)

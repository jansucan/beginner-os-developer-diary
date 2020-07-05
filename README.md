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
Also let me know if there is something you have a hard time
understanding either in this text or the source codes. I will try to
explain it better.

# What I already know

These are my most important sources of information about OSes so far

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

## Primary stage

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

It's done. I need to create a makefile to automate building of the
bootable image. I will use GNU Make, but I really need to read its
manual because I don't want to create it by copy-pasting and hope it
will work.

- [gnu: GNU make manual](https://www.gnu.org/software/make/manual/)
- [stackoverflow: Setting the stack registers](https://stackoverflow.com/a/45285701)
- [wiki osdev: x86 memory map](https://wiki.osdev.org/Memory_Map_(x86))

Done. The Qemu command for booting the image is

```
qemu-system-x86_64 -device piix3-usb-uhci \
                   -drive id=my_usb_disk,file=boot.img,if=none,format=raw \
                   -device usb-storage,drive=my_usb_disk
```

## Secondary stage

Next, I'm going to add support to the primary boot stage for loading a
bigger secondary stage. The secondary stage will be responsible to
seting up the environment for the kernel.

The primary stage has to be put somwhere where the BIOS can find it so
it can load it into the memory. With the secodary stage there is more
freedom in terms of where to put it.

I can think of three possibilities where to save the second stage image data

- File system. The size of the primary stage limits the choice
  of the file system. There is not enough space to support more
  complex file systems and I would like to implement a file system
  support in a higher level language.

- Raw partition. The size of the partition would mark a size of
  the second stage image. This information would need to be updated in
  the MBR.

- Raw device. Space for the second stage will be reserved after the
  MBR. Beginning of the first partition will need to be shifted a bit.

I chose the last option: raw device. I think it's the most simple
one. I'm going to write a "Hello, world!" second stage and get it
loaded by the primary stage.

- [stackoverflow: Debugging bootloader](https://stackoverflow.com/questions/14242958/debugging-bootloader-with-gdb-in-qemu)
- [stackoverflow: Single-step assembly code](https://stackoverflow.com/questions/2420813/using-gdb-to-single-step-assembly-code-outside-specified-executable-causes-error)
- [weinholt: Debugging boot sectors](https://weinholt.se/articles/debugging-boot-sectors/)

Done. The secondary stage is compiled first because the primary stage
needs to know how many sector the secondary stage image occupies so it
can load it.

It didn't work at the first time. From the messages printed it seemed
as if the primary stage was executed twice. I decided that it would be
a good time to learn some boot code debugging in Qemu. The bug was
really simple. I forgot that CHS sector numbering starts from 1 so the
primary stage was loading itself at the secondary stage offset and
executing it.

Now, when I can write more code in Assembly and I am not restricted to
the one sector, I want to explore how I could get to programming in C
as soon as possible.

- [wiki osdev: Bare bones](https://wiki.osdev.org/Bare_Bones)

The kernel in C should be executed in a protected mode of the CPU. I
need to get to know Intel CPUs better. I'm going to read through
Volume 3 of the *Intel® 64 and IA-32 Architectures Software
Developer’s Manuals*. It contains the full system programming
guide. There are 1766 pages in the document. It will probably take me
a few weeks to go through it.

- [software intel: SW developer manuals](https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html)

It seems that information from the first 10 chapters of the manual
will be sufficient for my goals. However, the bootloader needs to
prepare environment for the kernel running in protected mode
first. Two things need to be done:

- [wiki osdev: Enabling A20 line](https://wiki.osdev.org/A20_Line)
- [wiki osdev: Detecting memory](https://wiki.osdev.org/Detecting_Memory_(x86))

I'm going to write support for enabling A20 line first. After breaking
this task to smaller parts, the next step is to write a subroutine for
detecting, whether A20 line is enabled or not.

- [software intel: Instruction set reference](https://software.intel.com/content/www/us/en/develop/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-2a-2b-2c-and-2d-instruction-set-reference-a-z.html)
- [blog sleeplessbeastie: Convert raw image to VDI](https://blog.sleeplessbeastie.eu/2012/04/29/virtualbox-convert-raw-image-to-vdi-and-otherwise/)

Done. I was a bit surprised when I found out that in Qemu the A20 line
was already enabled. I wanted to be sure that my code for detecting
A20 works so I tested it in VirtualBox and A20 was disabled there. I
decided not to implement enabling of A20 yet and continue with
detecting memory instead.

The detected memory map should be passed to a kernel. Instead of my
own scheme for doing that I will use the Multiboot Specification.

- [gnu: Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)

I want to print the dected memory map on the screen so I can see
whether the detection works or not. However I would like to implement
the print functions in C, not in Assembly, but I have read that GCC
cannot generate 16-bit code reliably.

- [stackoverflow: How to tell GCC to generate 16-bit code for real mode](https://stackoverflow.com/questions/19055647/how-to-tell-gcc-to-generate-16-bit-code-for-real-mode)

Thus, the code has to be 32-bit. This implies switching the CPU to the
protected mode. I think, the next step will be to be able to load and
run 32-bit executable in a protected mode.

- [wiki osdev: Protected mode](https://wiki.osdev.org/Protected_Mode)

It's advised to disable non-maskable interrupt before entering to
protected mode. This should be done by writing to port 0x70. I'm going
to add this to the bootloader's second stage.

- [wiki osdev: Non-maskable interrupt](https://wiki.osdev.org/Non_Maskable_Interrupt)
- [bochs sourceforge: Port list](http://bochs.sourceforge.net/techspec/PORTS.LST)

Finally in the protected mode! However, I would like to understand the
process of switching CPU modes better. I'm going to read Chapter 9
*Processor management and initialization* in the Volume 3 of the Intel
manuals. I will also read through the similar chapter in AMD
manuals.

- [amd: AMD64 Architecture programmer's manual](https://www.amd.com/system/files/TechDocs/24593.pdf)

I need to build a cross-compiler. I want to use the latest GCC version
which is 10.1, but my GNU/Linux system contains version 5.5.0. It's
advised version of a cross-compiler to be the same as a version of the
compiler used to build it (the cross-compiler). So I'm going to look
into building the latest native compiler first. However, I need to
wait until I have a laptop cooling pad which I ordered a few days
ago. Without it my machine slows down when it's under heavy load,
which building a compiler certainly is, and is overheating.

- [wiki osdev: GCC cross-compiler](https://wiki.osdev.org/GCC_Cross-Compiler)

I have receiced the cooling pad so I can start building the latest
native compiler.

- [gcc gnu: Installing GCC](https://gcc.gnu.org/install/)

I have built the latest version of the native compiler, and then
binutils and cross-compilers for i686-elf and x86_64.

Hooray! I got to C language!

- [greek0: The ELF format - how programs look from the inside](https://greek0.net/elf.html)
- [stackoverflow: Load ELF file into memory ](https://stackoverflow.com/questions/39486061/load-elf-file-into-memory)
- [stackoverflow: Make executable binary file from ELF using GNU objcopy](https://stackoverflow.com/questions/19944441/make-executable-binary-file-from-elf-using-gnu-objcopy)

I used a simple kernel from

- [wiki osdev: Bare bones](https://wiki.osdev.org/Bare_Bones)

I'm going to implement better terminal output functions and then I can
get back to detecting memory map and passing it to the kernel
according to the multiboot specification. Note that the kernel is
32-bit because a multiboot bootloader can load only a 32-bit kernel. I
call it *init* part of the kernel. It's purpose is to setup an
environment for the main 64-bit part of the kernel.

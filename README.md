<h1>KFS1</h1>
<p>Kernel from sratch project: my final 42 branch of project</p>
<p>KFS1 is the first out of 10 project which leads you to create your very own kernel step by step</p>
<p>Write a kernel from scratch for x86 32bits arch</p>

<ul>
<li><p>Create a virtual disk image and install grub on it</p></li>
<li><p>link and Boot a kernel program minimal code written in asm and C with multiboot header specification</p></li>
<li><p>Write basic functions helper such as string functions from standard libc etc</p></li>
<li><p>Create a minimalistic interface between kernel and screen</p></li>
<li><p>Create a virtual disk image and install grub on it</p></li>
<li><p>Handle scroll and colors in vga standard text mode</p></li>
<li><p>Write a basic printf/printk</p></li>
<li><p>Handle several screens and shortcuts to switch between them</p></li>
</ul>

Linux xubuntu - 64 bit

<h2>Required packages and tools</h2>
<br />
<p>QEMU emulator version 2.11.1</p>
<p>gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0</p>
<p>GNU assembler (GNU Binutils for Ubuntu) 2.30</p>
<p>NASM version 2.13.02</p>
<p>GNU ld (GNU Binutils for Ubuntu) 2.30</p>
<p>GNU Make 4.1</p>
<p>losetup from util-linux 2.31.1</p>
<p>grub (grub-install (GRUB) 2.02-2ubuntu8.17)</p>
<p>mke2fs</p>
<p>fdisk from util-linux 2.31.1</p>

<h2>How to run the project</h2>
<p>From project's root dir, type <code>make</code> then <code>make install</code></p>
<p>installation require super user rights</p>

<h2>Shortcut</h2>
<ul>
<li><p><code>f2</code> - previous screen</p></li>
<li><p><code>f3</code> - next screen</p></li>
<li><p><code>esc</code> - shutdown</p></li>
</ul>
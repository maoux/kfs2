cp kfs.bin ./dest/boot
cp config/grub.cfg ./dest/boot/grub
grub-mkrescue -o kfs.iso ./dest
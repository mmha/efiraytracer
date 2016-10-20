efiraytracer
============

![Screenshot](/static/screenshot.png)

The title says it all: A proof of concept raytracer for bare metal x86_64 with an UEFI firmware. It's full of rough edges, but should render a sphere at 800x600 pixels using a PBR shader.

Running it
----------

If you want to run it on real hardware, download the .efi and start it with the UEFI shell or a bootloader of your choice. It is possible that your UEFI does not support the requested video mode, in which case you have to build it by yourself.

Your best chances are by running it in qemu with the OVMF Firmware: 
```
qemu-system-x86_64 -bios ovmf_x64.bin -hda raytracer.img
```

Building it
-----------

You need a Linux with a `x86_64-w64-mingw32-g++` cross compiler, GNU efilib and CMake. The build system will produce a EFI executable.

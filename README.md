efiraytracer
============

![Screenshot](/static/screenshot.png)

The title says it all: A proof of concept raymarcher for bare metal x86\_64 with an UEFI firmware. It's full of rough edges, but should render a sphere at 800x600 pixels using a PBR shader.

Running it
----------

If you want to run it on real hardware, download the .efi and start it with the UEFI shell or a bootloader of your choice. It is possible that your UEFI does not support the requested video mode, in which case you have to build it by yourself. This is required because the raytracer does not use dynamic memory allocation.

Your best chances are by running it in qemu with the OVMF Firmware (Take a look at [this tutorial](https://github.com/tianocore/tianocore.github.io/wiki/How-to-run-OVMF) for details): 
```
qemu-system-x86_64 -bios ovmf_x64.bin -hda raytracer.img
```

Building it
-----------

You need a Linux with a `x86_64-w64-mingw32-g++` cross compiler, GNU efilib and CMake. The build system will produce a EFI executable.

Create a `build` directory as usual, but select the CMake Toolchain for the UEFI target. Then build it using `make`:
```bash
mkdir build
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/efi_x86_64_gcc.cmake
make
```

The output will be a UEFI executable and can be used to boot a system.


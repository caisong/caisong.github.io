---
title: arm ports differences
date: 2017-09-13 22:25:00
tags: arm
category: embedded
---



## Three ports to different flavours of little-endian ARM hardware  
**The following content reproudced from Debian Wiki** [ArmPorts](https://wiki.debian.org/ArmPorts "Debian wiki")  
* The ARM EABI (armel) port targets a range of older 32-bit ARM devices, particularly those used in NAS hardware and a variety of *plug computers.
* The newer ARM hard-float (armhf) port supports newer, more powerful 32-bit devices using version 7 of the ARM architecture specification.
* The 64-bit ARM (arm64) port supports the latest 64-bit ARM-powered devices.

## Prebuilt toolchains
* Debian cross-tools packages
**The following content reproudced from elinux** [toolchains](http://www.elinux.org/Toolchains "toolchains")  
For Debian users, the toolchains problem is fairly reliably solved.
For a debian-based box just install pre-built cross toolchains from Debian experimental.
Targets include nearly all debian-supported architectures As of this writing supported compiler is gcc 4.9. You can get older unsupported compilers from emdebian.
You will need to add the target architecture to your list of installable architectures. eg. 
```shell
 dpkg --add-architecture armhf
 apt-get update
 apt-get install gcc-arm-linux-gnueabihf
```

## Toolchain building systems
* Bitbake
Bitbake is the tool used by [OpenEmbedded](http://wiki.openembedded.net/index.php/Main_Page "OpenEmbedded"). The best way to get started is probably by just building an existing distribution that uses openembedded.
---
title: arm ports differences
date: 2017-09-13 22:25:00
tags: arm
category: embedded
---



## Three ports to different flavours of little-endian ARM hardware  
**The following content reproudced from Debian Wiki** [ArmPorts](https://wiki.debian.org/ArmPorts "Debian wiki")  
* ArmPort - the first port, using the "old" (now obsolete) ABI (OABI). First released with 2.2 (Potato), last released with 5.0 (lenny). GNU Triplet: arm-linux-gnu
* ArmEabiPort - newer port using the "new" ABI (EABI), supported on Arm v4t and higher. First released with 5.0 (lenny). GNU Triplet: arm-linux-gnueabi
* ArmHardFloatPort - the latest 32-bit port, using the hard-float version of the "new" ABI (EABI), targeting Arm v7 and up. First released with 7 (wheezy). GNU Triplet: arm-linux-gnueabihf
* Arm64Port - the latest port, for the 64-bit Armv8 architecture. First released with 8 (jessie). GNU Triplet: aarch64-linux-gnu

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
 # arm64 gcc-r-aarch-linux-gnu
```

## Toolchain building systems
* Bitbake
Bitbake is the tool used by [OpenEmbedded](http://wiki.openembedded.net/index.php/Main_Page "OpenEmbedded"). The best way to get started is probably by just building an existing distribution that uses openembedded.
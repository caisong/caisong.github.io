---
title: Linux开机启动
date: 2021-09-03T21:04:38+08:00
lastmod: 2021-09-03T21:04:38+08:00
author: Cai Song
# avatar: /img/author.jpg
# authorlink: https://author.site
cover: /img/cover.jpg
# images:
#   - /img/cover.jpg
categories:
  - linux
tags:
  - linux
  - 开机启动
# nolastmod: true
draft: false
---


## /etc/inittab Line Syntax
```shell
id:rstate:action:process
```

## /etc/init  &&  /etc/init.d  

> `/etc/init.d` contains scripts used by the System V init tools (SysVinit). This is the traditional service management package for Linux, containing the init program (the first process that is run when the kernel has finished initializing¹) as well as some infrastructure to start and stop services and configure them. Specifically, files in /etc/init.d are shell scripts that respond to start, stop,  restart, and (when supported) reload commands to manage a particular service. These scripts can be invoked directly or (most commonly) via some other trigger (typically the presence of a symbolic link in `/etc/rc?.d/`).

>  `/etc/init` contains configuration files used by Upstart. Upstart is a young service management package championed by Ubuntu. Files in /etc/init are configuration files telling Upstart how and when to start, stop, reload the configuration, or query the status of a service. As of lucid, Ubuntu is transitioning from SysVinit to Upstart, which explains why many services come with SysVinit scripts even though Upstart configuration files are preferred. In fact, the SysVinit scripts are processed by a compatibility layer in Upstart.

> .d in directory names typically indicates a directory containing many configuration files or scripts for a particular situation (e.g. /etc/apt/sources.list.d contains files that are concatenated to make a virtual sources.list; /etc/network/if-up.d contains scripts that are executed when a network interface is activated). This structure is usually used when each entry in the directory is provided by a different source, so that each package can deposit its own plug-in without having to parse a single configuration file to reference itself. In this case, it just happens that “init” is a logical name for the directory, SysVinit came first and used init.d, and Upstart used plain init for a directory with a similar purpose (it would have been more “mainstream”, and perhaps less arrogant, if they'd used /etc/upstart.d instead).

## `service enable`

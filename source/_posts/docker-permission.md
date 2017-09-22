---
title: docker permission
date: 2017-09-22 22:34:59
tags: docker
---
# docker 权限控制
docker在映射文件时，默认会使用与映射目录相同uid:gid，经常会导致一些权限问题。
常见有以下解决方案：
* 改变权限
1. 创建容器后，进入后`chown`
2. 通过`docker run`的`-u`选项
  ```shell
  docker run -v `pwd`/shared:/shared -u `stat -c "%u:%g" /shared` ubuntu bash
  ```
这样做的话就等于放弃了root权限，除非再新建root组用户

* 使用Data-only container
[Data-only container madness](http://container42.com/2014/11/18/data-only-container-madness/ “data-only”)
主要手段如下:
1. 创建数据容器，改变目录用户权限。
2. 其他容器使用`--volumes-from`挂载创建的数据容器
```shell
docker run -v /userdata --name data busybox true
docker run --volumes-from data debian:jessie bash
```


---
title: docker install on windows
date: 2017-10-28 16:31:52
tags: docker
---

1. 安装
阿里云下载[docker toolbox](https://mirrors.aliyun.com/docker-toolbox/windows/docker-toolbox/)
2. 常见问题
* `error: Details: code E_FAIL (0x80004005), component MachineWrap, interface I `
  virtual box 版本较低导致，升级后问题解决
  P.S. 另一由于CPU不支持虚拟化的导致的问题没遇到

* 设置国内镜像仓库
  1. 创建之前指定
    ```shell
    
    docker-machine create --engine-registry-mirror=http://hub-mirror.c.163.com -d virtualbox default
    ```

  2. 创建之后，进入machine修改
    ```shell
    docker-machine ssh <machine-name>
    sudo -i
    sed -i '/^EXTRA_ARGS.*/ s/$/ ----registry-mirror=http://hub-mirror.c.163.com' /var/lib/boot2docker/profile
    /etc/init.d/docker restart
    ```
    P.S. 默认docker用户，用`sudo -i`切换root用户
  
  * 镜像路径
    默认镜像路径在用户目录下，在C盘  
    1. 安装前
    设置环境变量`MACHINE_STORAGE_PATH`指向你需要设置的目录
    2. 安装后
    P.S. 
      * 创建快捷方式的方法失败
      * 指定docker daemon graph的方法失败
      * 未尝试使用`ln`链接目录

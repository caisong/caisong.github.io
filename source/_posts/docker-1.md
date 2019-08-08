---
title: docker 记录2
date: 2019-08-08 22:52:43
tags: 
  - docker
  - cmder
categories: docker
---

上一年双十一购置了一台云服务器，就很少搭一些环境了。最近要搭建一套自动测试环境，公司内网缺少太多工具，就打算外网docker构建一套，再导入内网。
以上为背景。
----------------

## win7/win10差别

* **win7 使用docker toolbox**
  **win10 使用Dcoker For Windows**

* 国内用户可以选择阿里云hub加速器，说明很详细  
偷个懒，直接贴图
![aliyun_hub.jpg](/images/aliyun_hub.jpg "阿里云hub加速")
  



## 使用cmder作为终端
[参考链接](https://goblincoding.com/2016/05/24/adding-docker-terminal-to-cmder-on-windows/ "docker cmder")

![setuptasks.gif](https://goblincoding.files.wordpress.com/2016/05/setuptasks.gif?w=1100 "一图胜千言")

其实就是将docker工具创建的快捷方式中的命令拷贝到`cmder`中
```shell
"D:\Program Files\Git\bin\bash.exe --login -i "D:\Program Files\DockerToolbox\start.sh" -new_console:d:"D:\Program Files\DockerToolbox""
```

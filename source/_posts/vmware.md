---
title: vmware 记录
date: 2016-07-24 21:50:08
tags: vmware
category: record
---

## 此虚拟机正在使用中，取得该虚拟机的所有权失败
问题描述：该问题一般是由于强制关闭vmware进程导致
解决方案：你所配置的虚拟机目录下存在*.lck文件，删除就可以了。

## CentOS 启动没有IPV4 地址
问题描述：centos7虚拟机，没有ipv4地址，无法ssh连接
解决办法：默认设置没有开机激活网卡，修改网络配置文件
```
sudo vim /etc/sysconfig/network-scripts/ifcfg-enoXXX #修改ONBOOT=yes 
sudo systemctl restart network
```

## 虚拟机提示Failed to start LSB: Bring up/down networking
问题描述：网络无法连接，重启network提示该错误`Failed to start LSB: Bring up/down networking`。
* 搜索到的解决办法一般是由于拷贝他人的虚拟机文件导致，需要更改网络配置文件中的HWADDR字段，改为你自己mac地址。(针对CentOS7)
```
ip addr show #查看mac地址
sudo vim /etc/sysconfig/network-scripts/ifcfg-enoXXX #修改HWADDR字段
```
* 我遇到的问题是由于关闭了`VMware NAT Service` 服务导致，启动就可以了

谨此记录，日后再补充遇到的其他问题

## ssh问题
详见鄙人关于ssh问题的blog

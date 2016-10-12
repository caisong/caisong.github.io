---
title: ssh问题-Connection closed by foreign host
date: 2016-06-30 22:56:04
tags: ssh
category: linux
---

## 问题描述

使用XShell 登陆linux总是回复：`Connection closed by foreign host.` 或者
`Disconnected from remote host`，
造成该现象的原因一般是由于ssh连接长期没有操作导致的（这也是误导我的主要原因）

## 原因及解决方案

### 账号被限制只允许一个人登陆（暂时没遇到）

### 无法保持长期连接
```bash
sudo vim /etc/ssh/sshd_config
```
结尾添加两行
```
ClientAliveInterval 60
ClientAliveCountMax 3
```

```bash
# 重启服务
sudo systemctl restart sshd
```

### 防火墙原因

```bash
# 关闭防火墙
sudo systemctl stop sshd
# 或者打开22端口
sudo iptables -A INPUT -p tcp --dport 22 -j ACCEPT
```

### hosts.deny文件被篡改

查看`/etc/hosts.deny`文件，看是否被禁用
```
# 禁止所有ip访问linux 的ssh功能
sshd:all:deny
```
> `/etc/hosts.allow`（允许）和`/etc/hosts.deny`（禁止）这两个文件是tcpd服务器的配置文件，tcpd服务器可以控制外部IP对本机服务的访问。
> 系统会先检查`/etc/hosts.deny`规则，再检查`/etc/hosts.allow`规则，如果有冲突 按`/etc/hosts.allow`规则处理

### 客户端连接数过多

将`/etc/ssh/sshd_config`中 MaxStartUps 10 改为MaxStartups 10/100，重启sshd

### 针对VMware nat模式
描述：连接方式使用NAT；虚拟能在ping通主机，主机无法ping通虚拟机。
原因：虚拟机自动获取的IP与vmnet8设置的IP不在同一网段。
解决：设置vmnet8自动获取ip即可。



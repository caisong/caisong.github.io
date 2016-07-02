---
title: bat切换当前目录
date: 2016-06-20 21:35:20
tags: windows
category: record
---

```shell
@echo off
 cd %~dp0
 call git pull
 xcopy D:\google\hosts\hosts C:\Windows\System32\drivers\etc\ /Y /R
```
每次更新host，使用管理员运行显示git失败，原因是使用管理员运行时，目录切换到system32下了。
`cd %~dp0`表示切换回当前目录


---
title: atom config 设置
date: 2016-10-12 21:46:56
tags: atom
category: eidtor
---

在文件_~/.atom/.apmrc_中添加以下内容，若该文件不存在，则创建
```ini
strict-ssl=false
https-proxy=http://192.168.10.1:8080
http-proxy=http://192.168.10.1:8080
```
*http与proxy之间使用的是连字符，而不是下划线*

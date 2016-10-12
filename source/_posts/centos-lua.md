---
title: CentOS 安装Lua 5.3
date: 2016-10-12 21:47:19
tags: record
category: lua
---

按照官网教程，执行以下命令
```bash
curl -R -O http://www.lua.org/ftp/lua-5.3.3.tar.gz
tar zxf lua-5.3.3.tar.gz
cd lua-5.3.3
make linux test
```

但其实lua并未安装，许多博文都是建立软链接，但其实`makefile`文件中已经写好了，只需install就可以了
```bash
make linux install
```

升级lua版本之后，会导致`luarock`无法使用，需要重新编译安装。
```bash
wget http://luarocks.org/releases/luarocks-2.4.1.tar.gz
tar zxpf luarocks-2.4.1.tar.gz
cd luarocks-2.4.1
./configure; sudo make bootstrap
```

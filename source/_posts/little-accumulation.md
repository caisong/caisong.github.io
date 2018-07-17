---
title: little accumulation
date: 2018-07-17 22:22:52
tags: accumulation
---

好久没写博客了，决定重新抓起来。不一定要写多么深奥的内容，记录下收获就可以。

## rename 批量重命名文件 
rename 批量更改文件名
```bash
rename txt jpg *.txt
```

## timeout 指定命令运行时间
```bash
timeout -s KILL 60s ping 127.0.0.1
# DURATION 默认单位s
# 指令可以通过 kill -l 查看
```


## 编程技巧
    结构体中使用`char [0]`
    ```c
    struct MyData 
    {
        int nLen;
        char data[0];
    };
    ```

    在结构中，data是一个数组名，但该数组没有元素，该数组的真实地址紧随结构体MyData之后，而这个地址就是结构体后面数据的。*如果给这个结构体分配的内容大于这个结构体实际大小，后面多余的部分就是这个data的内容。*
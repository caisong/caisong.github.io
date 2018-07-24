---
title: vptr
date: 2018-07-24 22:57:44
tags: c++, vptr
categories: c++
---

之前无意中看见别人代码，在`struct` 中初始函数中，直接`memset`,如
```cpp
struct Content{
    char indexx[64];
    //....

    void init()
    {
        memset(this, 0, sizeof(Content));
    }
};
```

最近代码遇到成员变量较多，想采用类似方法，但程序总是崩溃，发生在基类析构函数。

打印发现**虚指针空**，因为基类析构函数一般时虚函数（原因请参见effectiv C++），子类中存在`vptr`。

摘用侯捷《度探索C++对象模型》中的实例

```cpp
class A {
public:
    virtual void vfunc1();
    virtual void vfunc2();
    void func1();
    void func2();
    virtual ~A();
private:
    int m_data1, m_data2;
}; 

class B : A {
public:
    virtual void vfunc1();;
    void func2();
    virtual ~B();
private:
    int m_data3;
};

class C : B {
public:
    virtual void vfunc1();
    void func();
private:
    int m_data1, m_data4;
};
```

![vprt.jpg](/images/vtpr.jpg "内存对象模型")

使用本文开头的方法`memset`初始化成员时，也同时将`vptr`置为空，导致析构时空指针异常崩溃。

可以在`memset`时偏移一个指针的长度。

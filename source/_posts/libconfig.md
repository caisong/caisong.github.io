---
title: libconfig 入门
date: 2017-12-25 22:59:18
tags: libconfig
categories: tools
---

以下内容翻译自[libconfig manual](http://www.hyperrealm.com/libconfig/libconfig_manual.html "libconfig manual")
版本1.4.9，最新版本请参阅[libconfig](https://hyperrealm.github.io/libconfig/ "libconfig")

# 应用示例
1. 配置文件
```plain
# Example application configuration file

version = "1.0"; //①

application:
{
  window:
  {
    title = "My Application";
    size = { w = 640; h = 480; };
    pos = { x = 350; y = 250; }; //②
  };

  list = ( ( "abc", 123, true ), 1.234, ( /* an empty list */) );

  books = ( { title  = "Treasure Island";
              author = "Robert Louis Stevenson";
              price  = 29.95;
              qty    = 5; },
            { title  = "Snow Crash"; //③
              author = "Neal Stephenson";
              price  = 9.99;
              qty    = 8; } );

  misc:
  {
    pi = 3.141592654;
    bigint = 9223372036854775807L;
    columns = [ "Last Name", "First Name", "MI" ];
    bitmask = 0x1FC3;
  };
};
```

2. 读取方式
|目标|路径|
|:--|:--|
|version ①|version|
|x ②|application.window.pos.x|
|title ③|application.books.[1].tittle|

路径按照元素名层层遍历，用'.'连接。类似数组和链表的情况，使用下标（从0开始）。**此处与C/C++不同，请注意 title ③的路径**

# 语法说明
|类型|说明|规则|正则表达|
|:--|:--|:--|:--|
|boolean|布尔类型|true/false,不区分大小写|([Tt][Rr][Uu][Ee])&#124;([Ff][Aa][Ll][Ss][Ee])|
|string|字符串|支持常见转义字符；支持换行；支持\x开头的ASCII转码|\"([^\"\\]&#124;\\.)*\"|
|integer|整数型||[-+]?[0-9]+|
|integer64|长整数||[-+]?[0-9]+L(L)?|
|hex|||0[Xx][0-9A-Fa-f]+|
|hex64|||0[Xx][0-9A-Fa-f]+L(L)?|
|float|浮点数|支持科学计数法|([-+]?([0-9]*)?\.[0-9]*([eE][-+]?[0-9]+)?)&#124;([-+]([0-9]+)(\.[0-9]*)?[eE][-+]?[0-9]+)|
|name|配置文件元素命名规则|以\*或者字母开头，可包含字母、数字、\*和下滑线|[A-Za-z\*][-A-Za-z0-9_\*]*|

tips: `&#124;`表示`|`

```plain
# 配置文件BNF表示

configuration = setting-list | empty

setting-list = setting | setting-list setting

# 配置文件中键值对叫setting
# 使用`name=value`或`name:value;`的形式，
# 不关心空格，';'可省略
setting = name (":" | "=") value (";" | "," | empty)

value = scalar-value | array | list | group

value-list = value | value-list "," value

scalar-value = boolean | integer | integer64 | hex | hex64 | float
               | string

scalar-value-list = scalar-value | scalar-value-list "," scalar-value

# 数组，*个scalar-value组成，成员scalar-valu必须具有相同类型
array = "[" (scalar-value-list | empty) "]"

# 链表，与数组类似，值类型无限定
list = "(" (value-list | empty) ")"

# 分组，可包含任意个setting,但setting的name必须唯一
group = "{" (setting-list | empty) "}"

empty =

```

# API
请参阅官方链接[C](https://hyperrealm.github.io/libconfig/libconfig_manual.html#The-C-API "C API")、[C++](https://hyperrealm.github.io/libconfig/libconfig_manual.html#The-C_002b_002b-API "C++ API")Y

# 官方示例

* 配置文件 example.cfg
```plain
// An example configuration file that stores information about a store.

// Basic store information:
name = "Books, Movies & More";

// Store inventory:
inventory =
{
  books = ( { title  = "Treasure Island";
              author = "Robert Louis Stevenson";
              price  = 29.99;
              qty    = 5; },
            { title  = "Snow Crash";
              author = "Neal Stephenson";
              price  = 9.99;
              qty    = 8; }
          );

  movies = ( { title = "Brazil";
               media = "DVD";
               price = 19.99;
               qty = 11; },
             { title = "The City of Lost Children";
               media = "DVD";
               price = 18.99;
               qty = 5; },
             { title = "Memento";
               media = "Blu-Ray";
               price = 24.99;
               qty = 20;
             },
             { title = "Howard the Duck"; }
           );
};

// Store hours:
hours =
{
  mon = { open =  9; close = 18; };
  tue = { open =  9; close = 18; };
  wed = { open =  9; close = 18; };
  thu = { open =  9; close = 18; };
  fri = { open =  9; close = 20; };
  sat = { open =  9; close = 20; };
  sun = { open = 11; close = 16; };
};
```

* 代码 example1.cpp 
```cpp
/* ----------------------------------------------------------------------------
   libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner

   This file is part of libconfig.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, see
   <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------------
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

// This example reads the configuration file 'example.cfg' and displays
// some of its contents.

int main(int argc, char **argv)
{
  Config cfg;

  // Read the file. If there is an error, report it and exit.
  try
  {
    cfg.readFile("example.cfg");
  }
  catch(const FileIOException &fioex)
  {
    std::cerr << "I/O error while reading file." << std::endl;
    return(EXIT_FAILURE);
  }
  catch(const ParseException &pex)
  {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    return(EXIT_FAILURE);
  }

  // Get the store name.
  try
  {
    string name = cfg.lookup("name");
    cout << "Store name: " << name << endl << endl;
  }
  catch(const SettingNotFoundException &nfex)
  {
    cerr << "No 'name' setting in configuration file." << endl;
  }

  const Setting& root = cfg.getRoot();

  // Output a list of all books in the inventory.
  try
  {
    const Setting &books = root["inventory"]["books"];
    int count = books.getLength();

    cout << setw(30) << left << "TITLE" << "  "
         << setw(30) << left << "AUTHOR" << "   "
         << setw(6) << left << "PRICE" << "  "
         << "QTY"
         << endl;

    for(int i = 0; i < count; ++i)
    {
      const Setting &book = books[i];

      // Only output the record if all of the expected fields are present.
      string title, author;
      double price;
      int qty;

      if(!(book.lookupValue("title", title)
           && book.lookupValue("author", author)
           && book.lookupValue("price", price)
           && book.lookupValue("qty", qty)))
        continue;

      cout << setw(30) << left << title << "  "
           << setw(30) << left << author << "  "
           << '$' << setw(6) << right << price << "  "
           << qty
           << endl;
    }
    cout << endl;
  }
  catch(const SettingNotFoundException &nfex)
  {
    // Ignore.
  }

  // Output a list of all books in the inventory.
  try
  {
    const Setting &movies = root["inventory"]["movies"];
    int count = movies.getLength();

    cout << setw(30) << left << "TITLE" << "  "
         << setw(10) << left << "MEDIA" << "   "
         << setw(6) << left << "PRICE" << "  "
         << "QTY"
         << endl;

    for(int i = 0; i < count; ++i)
    {
      const Setting &movie = movies[i];

      // Only output the record if all of the expected fields are present.
      string title, media;
      double price;
      int qty;

      if(!(movie.lookupValue("title", title)
           && movie.lookupValue("media", media)
           && movie.lookupValue("price", price)
           && movie.lookupValue("qty", qty)))
        continue;

      cout << setw(30) << left << title << "  "
           << setw(10) << left << media << "  "
           << '$' << setw(6) << right << price << "  "
           << qty
           << endl;
    }
    cout << endl;
  }
  catch(const SettingNotFoundException &nfex)
  {
    // Ignore.
  }

  return(EXIT_SUCCESS);
}

// eof

```

* 编译
```shell
#lib c++(c库libconfig-dev)
apt-get install libconfig++-dev

g++ example1.cpp -o example -lconfig++
```



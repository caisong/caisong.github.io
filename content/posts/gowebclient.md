---
title: Gowebclient
date: 2024-12-20T21:40:56+08:00
lastmod: 2024-12-20T21:40:56+08:00
author: Cai Song
# avatar: /img/author.jpg
# authorlink: https://author.site
cover: /img/cover.jpg
# images:
#   - /img/cover.jpg
categories:
  - golang
tags:
  - webdav
# nolastmod: true
draft: false
---

go webdav client示例
 ```golang
package main

import (
  "github.com/studio-b12/gowebdav"
  "fmt"
)

func main(){
  cli:=gowebdav.NewAuthClient("http://127.0.0.1:10080", gowebdav.NewEmptyAuth())
  if err:=cli.Connect();err!=nil{
    panic(err)
  }
  entries, err:= cli.ReadDir("/")
  if err!=nil{
    panic(err)
  }
  for _, entry:= range entries {
    if entry.IsDir() {
      fmt.Println("Dir ", entry.Name())
    }else{
      fmt.Println("File ", entry.Name())
    }
  }
}
 ```
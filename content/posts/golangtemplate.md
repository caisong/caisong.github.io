---
title: Golangtemplate
date: 2024-07-29T21:40:54+08:00
lastmod: 2024-07-29T21:40:54+08:00
author: Cai Song
# avatar: /img/author.jpg
# authorlink: https://author.site
cover: /img/cover.jpg
# images:
#   - /img/cover.jpg
categories:
  - category1
tags:
  - tag1
  - tag2
# nolastmod: true
draft: false
---

Cut out summary from your post content here.

<!--more-->

The remaining content of your post.
# golang template

 ```golang
 //模板文件
 {{- range . }}
host: {{ .Host }}
  {{- if eq .Category "DataBase" }}
    {{- if eq .Role "PostgreSQL" }}
  task: check postgresql connection
  cmd.database.dbtool:
    name: pg
    args:
	  port: {{ .DBPort }}
	  user: {{ .DBUser }}
	  passwd: {{ .DBPasswd }}
      dbname : {{ .DBName }}
    {{- else if eq .Role "MongoDB" }}
  task: check mongodb connection
  cmd.database.dbtool:
    name: mongo
    args:
      port: {{ .DBPort }}
      user: {{ .DBUser }}
      passwd: {{ .DBPasswd }}
    {{- else if eq .Role "Redis" }}
  task: check redis connection
  cmd.database.dbtool:
    name: redis
    args:
      port: {{ .DBPort }}
      passwd: {{ .DBPasswd }}
    {{- end }}
  {{- end }}
  {{- if eq .Category "Server" }}
    {{- if eq .Role "FDP" }}
  task: check fdp server
  cmd.server.tool:
    name: fdp
    args:
      - arg1
      - arg2
    {{- end }}
  {{- end }}
  {{- if eq .Category "Web" }}
    {{- if eq .Role "FullTextWeb" }}
  task: check full text web
  cmd.web.tool:
    name: fulltext
    args:
    {{- end }}
  {{- end }}
{{- end }}
 ```

 ```golang
 package main

import (
	"log"
	"os"
	"text/template"
)

type RoleInfo struct {
	Role, Category, DBName, Host, DBPort, DBUser, DBPasswd   string
	SshRequired, SudoRequired, FudeRequired, InstallRequired bool
}

func main() {
	log.SetFlags(log.Lshortfile | log.Ltime)
	roles := []RoleInfo{
		{
			"PostgreSQL", "DataBase", "postgres", "172.16.10.21",
			"5432", "postgres", "123456",
			true, true, true, false,
		},
		{
			"MongoDB", "DataBase", "test", "172.16.10.22",
			"27017", "mongo", "123456",
			true, true, false, false,
		},
		{
			"Redis", "DataBase", "", "172.16.10.23",
			"6379", "", "123456",
			true, false, false, false,
		},
		{
			"FDP", "Server", "", "172.16.10.31",
			"", "", "",
			true, true, false, false,
		},
		{
			"FullTextWeb", "Web", "", "172.16.10.41",
			"", "", "",
			true, true, true, true,
		},
	}
	tmpl, err := template.ParseFiles("task.tpl")
	if err != nil {
		log.Fatal(err)
	}
	err = tmpl.Execute(os.Stdout, roles)
	if err != nil {
		log.Fatal(err)
	}
}
 ```
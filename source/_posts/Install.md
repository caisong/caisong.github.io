---
title: PostgreSQL 安装
date: 2016-10-12 21:13:50
tags:
	- POstgreSQL
categories: DataBase
---
<!-- MarkdownTOC -->

- 安装
- 添加用户和数据库
- 登陆数据库
- 常用控制台命令
- 基本SQL操作
- 备份与恢复

<!-- /MarkdownTOC -->
<!--more-->
本文内容基于CentOS7及PostgreSQL 9.5.4
## 安装
以下内容主要参考[Linux downloads (Red Hat family)](https://www.postgresql.org/download/linux/redhat/ "postgresql安装")
* Included in distribution
目前CentOS7支持的是9.2版本，SCL源支持9.5与9.4
```bash
yum install postgresql-server posggresql-contrib -y
# 安装完成之后初始化
postgresql-setup initdb
systemctl enable postgresql.service systemctl start postgresql.service
```

* PostgreSQL Yum Repository
```bash
yum install http://yum.postgresql.org/9.5/redhat/rhel-7-x86_64/pgdg-redhat95-9.5-2.noarch.rpm
yum install postgresql95-server postgresql95-contrib -y
systemctl enable postgresql-9.5.service
systemctl start postgresql-9.5.service
```
* 其他可以通过下载rpm或编译源码的方式安装  

## 添加用户和数据库
以下内容主要参考[PostgreSQL新手入门](http://www.ruanyifeng.com/blog/2013/12/getting_started_with_postgresql.html "PostgreSQL新手入门")
安装PostgreSQL时，默认创建了用户名为postgres的系统用户和对应的DB管理员用户，可以直接登陆数据库。下面介绍如何添加用户自定义用户
### 方式一
该方式比较繁琐。  

1. 切换到postgres用户  
```bash
sudo su - postgres
```
2. 登陆DB
```bash
psql
```
3.
登陆数据库之后，系统提示符变为"postgres=#"。第一次登陆，需要设置下postgres管理员用户密码
```bash
\password postgres
```
4. 创建新DB用户
```bash
create user {username} with password 'password';
```
> * {username}需要指定为Linux系统用户名，可通过`createuser`方式创建（未指定为系统用户，在登陆时会失败，可能是用户role的原因）
> * 末尾 _;_ 不能缺少  
> * 创建完成之后，可通过`\du`命令列出所有用户，校验是否创建成功

5. 创建数据库test,并指定其所有者为dbuser
```bash
create database test owner dbuser;
```
> 创建完成之后，可通过`\l`命令查看所有数据库

6. 将test数据库的所有权限赋予dbuser
```bash
grant all privileges on database test to dbuser
```

### 方式二
1. 创建数据库用户dbuser，并指定其角色为超级用户
```bash
sudo -u posggres createuser --superuser dbuser
```
2. 登陆数据库控制台，设置用户密码后退出
```bash
sudo -u postgres psql
\password dbuser {password}
\q
```
3. 创建数据库，并指定其Owner为dbuser
```bash
sudo -u postgres createdb -O dbuser test
```

## 登陆数据库
与大多数数据库类似，指定用户名、密码及数据库地址，登陆数据库
```bash
psql -U {user_name} -d {db_name} -h {server_addr} -p {port_num}
```
若PostgreSQL用户中存在当前系统用户时，可以省略-U参数

## 常用控制台命令
```bash
\h                  查看帮助
\password           设置密码
\l                  列出所有数据库
\c [database_name]  切换数据库
\d                  列出当前数据库的所有表格
\d [table_name]     查看表结构
\du                 列出所有用户
\e                  打开文本编辑器
\conninfo           列出当前数据库和连接的信息
\q                  退出
```

## 基本SQL操作
与一般SQL语言一致

## 备份与恢复
* pg_dump 备份数据库到文本文件或其他指定格式文件，其参数与psql基本一致
* pg_dumpall 备份数据库到可执行sql文件
* pg_restore 恢复`pg_dump`生成的文件到数据库
* psql < sql文件 执行sql文件

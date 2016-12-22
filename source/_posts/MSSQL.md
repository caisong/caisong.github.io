---
title: MSSQL遇到问条记录
date: 2016-07-24 22:10:46
tags: MSSQL, SQL Server
category: DataBase
---
<!-- MarkdownTOC -->

- 登陆连接失败
- 启动sa登陆
- 服务器改为IP地址登陆
- 高版本数据库恢复到低版本数据库
- 自增长列插入指定值
- select into from 与 insert into select 用法与区别
- 总结

<!-- /MarkdownTOC -->
<!-- more -->

记录使用MSSQL使用过程中遇到的问题。新手，大多数搜索解决，全部实际操作过，如果不能解决您的问题，请见谅。
ps. 自己电脑没装MSSQL，因此没有截图。

## 登陆连接失败
错误描述：未找到或无法访问服务器.请验证实例名称是否正确并且SQL SERVER已配置允许远程链接.(provide:命名管道提供程序,error:40 -无法打开到SQL Server的连接)(Microsoft SQL Server,错误:2)
解决办法：
1. 开始菜单，SQL Server 20XX目录下找到 `SQL Server Configuration Manager`
2. SQL Server 【Version】 服务，启动
> * 可能存在多个服务，请启动对应你连接名的那个服务
> * 如果启动失败，请选择属性——登陆——本地系统账户，再次重新启动

## 启动sa登陆
问题描述：一般本地使用的windows账户，特别第一次安装时没注意，就没用设置sa密码，也就无法sa登陆
解决办法：
1. windows登陆
2. 在连接上右击，属性——安全性——SQL Server 和windows身份验证模式。保存，返回
3. 展开连接，安全性——登录名——sa，设置密码。切换到状态，登陆选择`启用`

## 服务器改为IP地址登陆
问题描述：一般安装完成之后，采用的是windows主机名+服务名的连接方式，而非IP地址，其他电脑无法访问
解决办法：
1. 启动sa登陆
2. 开始菜单，SQL Server 20XX目录下找到 `SQL Server Configuration Manager`
3. SQL Server 【Version】网络配置，启用`Namedpipes`和`TCP/IP`
4. `TCP/IP`属性，IP地址，配置##IP1##地址（IP2本地），##IPALL##TCP端口
5. 重启SQL Server 服务，连接

## 高版本数据库恢复到低版本数据库
问题描述： 高版本server生成的备份文件无法在低版本上恢复
解决办法：
### 无法能够连接到高版本服务器
我没有找到其他办法，只能升级低版本服务器

### 能够连接到高版本服务器
整体思路：
1. 在高版本服务器上，导出低版本服务器兼容的数据库创建sql语句
2. 低版本服务器执行，生成表
3. 利用SQL server数据导入工具，从高版本服务器导入数据。
   当然，如果数据量比较小，可以在导出SQL语句的时候，直接连数据一起导出，再执行。
   具体操作：
4. 导出SQL
> 1. 待备份数据库右击——任务——生成脚本
> 2. `设置脚本选项`——`为服务器版本编写脚本`，选择目标数据库版本
> 3. 如果要同时导出数据，`要编写脚本的数据的类型`选择`架构和数据`

2. 执行SQL
> 要先创建目标数据库，直接将SQL拖过去执行

3. 导出导出数据
> 数据库右击——导入数据，按提示下一步，`编辑映射`时选择`启用标识插入`

补充：高版本导入低版本数据，可以直接导入。但是，**表格的一些字段约束会丢失**。

## 自增长列插入指定值
问题描述：想要显示插入自动编号`IDENTITY`值，导致错误`结果会导致错误：当 IDENTITY_INSERT 设置为 OFF 时，不能向表 XXXX中的标识列插入显式值。`

```SQL
CREATE TABLE products (id int IDENTITY PRIMARY KEY, product varchar(20))
INSERT INTO products (id, product) VALUES(4, 'hello kitty') #返回错误
```

解决办法：
```SQL
SET IDENTITY_INSERT products ON
INSERT INTO products (id, product) VALUES(5, 'hello kitty')
```

## select into from 与 insert into select 用法与区别
select into from 和 insert into select都是用来复制表，两者的主要区别为： select into from 要求目标表不存在，因为在插入时会自动创建。insert into select from 要求目标表存在。

### SQL语法
+ Insert into Table2(field1,field2,...) select value1,value2,... from Table1
+ SELECT vale1, value2 into Table2 from Table1

### 实例
具体实例请查阅该文章[select into from 与 insert into select 用法与区别](http://www.studyofnet.com/news/182.html "select into from 与 insert into select 用法与区别")

## 总结
MSSQL 真心没有MySQL好用啊！！

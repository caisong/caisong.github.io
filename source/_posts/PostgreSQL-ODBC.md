---
title: PostgreSQL ODBC
date: 2016-10-12 21:32:48
tags:
	- postgresql
	- odbc
categories: DataBase
---
<!-- MarkdownTOC -->

- 安装unixODBC
- 安装PostgreSQL ODBC驱动
- 检查驱动配置
- 添加数据源
- 修改PostgrsSQL认证方式
- 验证
- ODBC 连接数据库

<!-- /MarkdownTOC -->
<!--more -->

## 安装unixODBC

直接下载官网的源码编译安装就可以了，比较简单

```bash
curl -O http://www.unixodbc.org/unixODBC-2.3.4.tar.gz
tar xvf unixODBC-2.3.4.tar.gz
cd unixODBC-2.3.4
./configure --sysconfdir=/etc
make
make install
```
或者直接
```bash
yum install unixODBC
```

## 安装PostgreSQL ODBC驱动

按照之前添加源的方式安装PostgreSQL，现在可以直接安装ODBC驱动.<br>
添加源方法

```bash
yum install http://yum.postgresql.org/9.5/redhat/rhel-7-x86_64/pgdg-redhat95-9.5-2.noarch.rpm
```
<!--more-->
安装驱动

```bash
yum install postgresql95-odbc postgresql95-odbc-debuginfo -y
```

## 检查驱动配置

```bash
odbcinst -q -d
```

若没有发现`PostgreSQL`相关的驱动项,通过`odbcinst -j`查看odbc配置信息，再查看对应_Drivers_项对应文件内容. 文件内容如下：

```plain
[ODBC Driver for PostgresSQL]
Description = ODBC Driver for PostgreSQL
Driver = /usr/pgsql-9.5/lib/psqlodbc.so
Setup = /usr/lib64/libodbcpsqlS.so
FileUsage = 1
```

若`odbcinst.ini`文件为空，可手动更改

## 添加数据源

根据`odbcinst -j`查询的配置，修改`SYSTEM DATA SOURCES`对应的数据配置文件

```plain
[mydb]
Description         = PostgreSQL connection to SallyDB
Driver              = ODBC Driver for PostgresSQL
Database            = caisong
Servername          = 127.0.0.1
UserName            = caisong
Password            = 123456
Port                = 5432
Protocol            = 9.5
ReadOnly            = No
RowVersioning       = No
ShowSystemTables    = No
ConnSettings        =
```

**Driver 项的值务必与驱动配置文件下的值一致**

## 修改PostgrsSQL认证方式

PostgreSQL默认本地认证方式为`peer`，远程连接认证方式为`ident` 具体差别，请参照[auth-methods](https://www.postgresql.org/docs/9.6/static/auth-methods.html "auth-methods") 修改host-based authentication认证文件，其路径配置在DB中，可通过SQL查询

```sql
select name, setting from pg_settings where category='File Locations';
```

默认配置如下：

```plain
name        |                 setting
-------------------+-----------------------------------------
config_file       | /var/lib/pgsql/9.5/data/postgresql.conf
data_directory    | /var/lib/pgsql/9.5/data
external_pid_file |
hba_file          | /var/lib/pgsql/9.5/data/pg_hba.conf
ident_file        | /var/lib/pgsql/9.5/data/pg_ident.conf
(5 rows)
```

修改`# IPv4 local connections` 下一行中的~~ident~~ 为 _md5_就可以了

```plain
# IPv4 local connections:
host    all             all             127.0.0.1/32            md5
```

修改完成之后需要重新加载配置文件 可以使用`pg_ctl reload`命令，默认该命令不在`PATH`下，其路径为`/usr/pgsql-9.5/bin/`,或者直接重启服务

```bash
systemctl restart postgresql-9.5
```

## 验证

```bash
isql mydb
```

如果出现错误，添加`-v`重新执行命令，此时能显示具体错误了。

## ODBC 连接数据库

```c
/*
 * testing unixODBC
 * the code from unixODBC website
 */
#include <stdlib.h>
#include <stdio.h>
#include <odbc/sql.h>
#include <odbc/sqlext.h>
#include <odbc/sqltypes.h>

SQLHENV V_OD_Env;                   // Handle ODBC environment
long V_OD_erg;                      // result of functions
SQLHDBC V_OD_hdbc;                  // Handle connection

char V_OD_stat[10];                 // Status SQL
SQLINTEGER V_OD_err,V_OD_rowanz,V_OD_id;
SQLSMALLINT V_OD_mlen,V_OD_colanz;
char V_OD_msg[200],V_OD_buffer[200];


int main(int argc,char *argv[])
{
        SQLHSTMT V_OD_hstmt;
        // 1. allocate Environment handle and register version
        V_OD_erg=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&V_OD_Env);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Error AllocHandle\n");
                exit(0);
        }
        V_OD_erg=SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Error SetEnv\n");
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        // 2. allocate connection handle, set timeout
        V_OD_erg = SQLAllocHandle(SQL_HANDLE_DBC, V_OD_Env, &V_OD_hdbc);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Error AllocHDB %d\n",V_OD_erg);
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        SQLSetConnectAttr(V_OD_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER *)5, 0);
        // 3. Connect to the datasource "mydb"
        V_OD_erg = SQLDriverConnect(V_OD_hdbc,
                                    NULL,
                                    "DRIVER={ODBC Driver for PostgresSQL};Server=127.0.0.1;Port=5432;Database=caisong;Uid=caisong;Pwd=123456;",
                                    SQL_NTS,
                                    V_OD_buffer,
                                    200,
                                    &V_OD_mlen,
                                    SQL_DRIVER_NOPROMPT);
        //a. direct connect into datasource
        /*V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*) "mydb", SQL_NTS,
                                         (SQLCHAR*) "caisong", SQL_NTS,
                                        (SQLCHAR*) "123456", SQL_NTS);
         */
        //b. manual connect into target db
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Error SQLConnect %d\n",V_OD_erg);
                SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1,
                              V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
                printf("%s (%d)\n",V_OD_msg,V_OD_err);
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        printf("Connected !\n");
        V_OD_erg=SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Fehler im AllocStatement %d\n",V_OD_erg);
                SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
                printf("%s (%d)\n",V_OD_msg,V_OD_err);
                SQLDisconnect(V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        SQLBindCol(V_OD_hstmt,1,SQL_C_CHAR, &V_OD_buffer,150,&V_OD_err);
        SQLBindCol(V_OD_hstmt,2,SQL_C_ULONG,&V_OD_id,150,&V_OD_err);

        V_OD_erg=SQLExecDirect(V_OD_hstmt,"SELECT name, setting FROM pg_settings where category='File Locations'",SQL_NTS);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Error in Select %d\n",V_OD_erg);
                SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
                printf("%s (%d)\n",V_OD_msg,V_OD_err);
                SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
                SQLDisconnect(V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        V_OD_erg=SQLNumResultCols(V_OD_hstmt,&V_OD_colanz);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
                SQLDisconnect(V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        printf("Number of Columns %d\n",V_OD_colanz);
        V_OD_erg=SQLRowCount(V_OD_hstmt,&V_OD_rowanz);
        if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
        {
                printf("Number ofRowCount %d\n",V_OD_erg);
                SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
                SQLDisconnect(V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
                SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
                exit(0);
        }
        printf("Number of Rows %d\n",V_OD_rowanz);
        V_OD_erg=SQLFetch(V_OD_hstmt);
        while(V_OD_erg != SQL_NO_DATA)
        {
                printf("Result: %d %s\n",V_OD_id,V_OD_buffer);
                V_OD_erg=SQLFetch(V_OD_hstmt);
        };
        SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
        SQLDisconnect(V_OD_hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        return(0);
}
```
全文完

layout: '''mssql'
title: transfer mssql data to mongodb'
date: 2017-09-05 19:56:02
tags: mssql, mongodb
category: database
---

写得比较基础

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-


import pymssql
import pymongo

sql = '''SELECT [Name]
      ,[CardNo]
      ,[Descriot]
      ,[CtfTp]
      ,[CtfId]
      ,[Gender]
      ,[Birthday]
      ,[Address]
      ,[Zip]
      ,[Dirty]
      ,[District1]
      ,[District2]
      ,[District3]
      ,[District4]
      ,[District5]
      ,[District6]
      ,[FirstNm]
      ,[LastNm]
      ,[Duty]
      ,[Mobile]
      ,[Tel]
      ,[Fax]
      ,[EMail]
      ,[Nation]
      ,[Taste]
      ,[Education]
      ,[Company]
      ,[CTel]
      ,[CAddress]
      ,[CZip]
      ,[Family]
      ,[Version]
  FROM cdsgus'''

ms_conn = pymssql.connect(host="127.0.0.1", user="sa",
                          password="root", database="database", charset="utf8")
ms_cur = ms_conn.cursor(as_dict=True)
ms_cur.execute(sql)

row = ms_cur.fetchone()

mongo_conn = pymongo.MongoClient('127.0.0.1', 27017)
mongo_db = mongo_conn['test']

while row:
    mongo_db.kf_data.insert_one(row)
    row = ms_cur.fetchone()

ms_conn.commit()
ms_conn.close()

```


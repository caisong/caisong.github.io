---
layout: post
title: MongoDB简要总结
tags:  mongodb mongodb优化
categories: mongodb
---
<div class="toc"></div>
# Mongodb 简要总结

## MongoDB 安装
1. 下载mongodb
不要下载legacy版本:[mongodb下载](https://www.mongodb.org/downloads "mongodb download")

2. 安装配置
* zip压缩包直接解压即可
* msi文件直接安装 or 通过以下命令指定安装目录
~~~
msiexec.exe /q /i mongodb-win32-x86_64-2008plus-ssl-3.0.6-signed.msi ^
            INSTALLLOCATION="C:\mongodb" ^
            ADDLOCAL="all"
~~~

## MongoDB启动配置
mongodb配置可以直接跟随命令行启动，或者通过YAML文件配置
1. 命令行方式
~~~
mongod --dbpath c:\data\db --port 27017 --logpath c:\data\log\mongod.log --directoryperdb
~~~
2. YAML文件
~~~
mongod -f config.yml
~~~
YAML文件示例
~~~YAML
systemLog:
    destination: file
    path: c:\data\log\mongod.log
storage:
    dbPath: c:\data\db
    directoryPerDB: true
net:
   port: 27017
~~~

## Mongo连接
~~~
mongo <host>:<port>
~~~
mongo可以通过两种方式执行外部JS代码
1. `mongo <host>:<port> <js_path>`
2. 用户目录下 **.mongorc.js**文件

## MongoDB Linux优化设置
* 关闭NUMA
~~~bash
echo 0 > /proc/sys/vm/zone_reclaim_mode
numactl --interleave=all ${MONGODB_HOME}/bin/mongod --config conf/mongodb.conf
~~~
* 关闭数据库文件的 atime
修改fstab
> 1. 在/etc/fstab 文件中添加noatime参数，例如：
    ~~~bash
    /dev/xvdb /data ext4 defaults 0 0
    ~~~
    更改为
    ~~~bash
    /dev/xvdb /data ext4 defaults,noatime 0 0
    ~~~
  2. 重新挂载分区
    ~~~bash
    mount -o remount /data
    ~~~
>  
如果不想改fstab，可以直接用mount命令
~~~bash
mount -o noatime -o nodiratime -o remount /data
~~~
* 推荐ulimit设置
~~~bash
ulimit -f unlimited  # (file size)
ulimit -t unlimited  # (cpu time)
ulimit -v unlimited  # (virtual memory size)
ulimit -n 64000      # (open files)
ulimit -u 64000      # (processes/threads)
~~~
* 使用NTP时间服务器

## MongoDB Windows优化设置
Windows 补丁：KB2731284

# MongoDB 常用操作
## Database Collection操作
Database, Collection都无需提前创建，在插入第一条记录时，会自动创建对应的db和collection

## DB操作
1. 切换DB：`use <dbname>`
2. 列出DB：`show dbs` 
3. 删除DB：`db.dropDatabase()`

## Collection操作
1. 列出Collection：`show tables`
2. 删除Collection：`db.collection.drop()`

## 常用CURD操作
## insert document
~~~JavaScript
db.inventory.insert(
   {
     item: "ABC1",
     details: {
        model: "14Q3",
        manufacturer: "XYZ Company"
     },
     stock: [ { size: "S", qty: 25 }, { size: "M", qty: 50 } ],
     category: "clothing"
   }
)
~~~
## find document
1. 匹配所有记录 
~~~JavaScript
db.inventory.find( {} )
~~~
2. 指定条件
~~~JavaScript
db.inventory.find( { type: "snacks" } )
~~~
3. 返回指定字段
~~~JavaScript
//只返回_id,item
//想要不返回_id字段，必须强制指定 _id:0
db.inventory.find( { type: "snacks" },{item:1})
//返回除item的所有字段
db.inventory.find( { type: "snacks" },{item:0})
~~~
4. 查询操作符
~~~JavaScript
db.inventory.find( { type: { $in: [ 'food', 'snacks' ] } } )
~~~
5. and运算符
~~~JavaScript
db.inventory.find( { type: 'food', price: { $lt: 9.95 } } )
~~~
6. or运算符
~~~JavaScript
db.inventory.find(
   {
     $or: [ { qty: { $gt: 100 } }, { price: { $lt: 9.95 } } ]
   }
)
~~~
7. and 和 or 运算符
~~~JavaScript
db.inventory.find(
   {
     type: 'food',
     $or: [ { qty: { $gt: 100 } }, { price: { $lt: 9.95 } } ]
   }
)
~~~
8. 内嵌文档
~~~JavaScript
db.inventory.find(
    {
      producer:
        {
          company: 'ABC123',
          address: '123 Street'
        }
    }
)
~~~
或者
~~~JavaScript
db.inventory.find( { 'producer.company': 'ABC123' } )
~~~
9. skip, limit, sort
示例：
~~~JavaScript
db.collection.find().skip(n).limit(n).sort({_id:1})
~~~
> * 三者顺序不影响查询结果；
> * 避免大范围skip
> * sort中_id:1表示升序,_id:-1表示降序
> * sort排序大小有限制，最大32M


## Modify document
修改document必须使用修改操作符，否则会**覆盖整个记录**
1. upadte document
~~~JavaScript
db.inventory.update(
    { item: "MNO2" },
    {
      $set: {
        category: "apparel",
        details: { model: "14Q3", manufacturer: "XYZ Company" }
      },
      $currentDate: { lastModified: true }
    }
)
~~~
2. replace document
~~~JavaScript
db.inventory.update(
   { item: "BE10" },
   {
     item: "BE05",
     stock: [ { size: "S", qty: 20 }, { size: "M", qty: 5 } ],
     category: "apparel"
   }
)
~~~
3. upsert和批量操作
update函数原型
~~~JavaScript
db.collection.update(
   <query>,
   <update>,
   {
     upsert: <boolean>,
     multi: <boolean>,
     writeConcern: <document>
   }
)
~~~
upsert：无匹配记录时，是否添加一条新纪录,默认false
multi:是否批量修改，默认false  
操作示例 
~~~JavaScript
db.collection.update( { "_id.name": "Robert Frost", "_id.uid": 0 },
   { "categories": ["poet", "playwright"] },
   { upsert: true } )
~~~

## remove docuemnt
1. **删除所有文档，若非必要，请勿执行**
~~~JavaScript
db.inventory.remove({})
~~~
2. 删除匹配的document 
~~~JavaScript
db.inventory.remove( { type : "food" } )
~~~
3. 删除匹配的单个document
~~~JavaScript
db.inventory.remove( { type : "food" }, 1 )
~~~

## index 
1. 创建索引
~~~JavaScript
db.collection.createIndex(keys, options)
~~~
普通索引
~~~JavaScript
db.collection.createIndex( { orderDate: 1, zipcode: -1 } )
~~~
唯一索引
~~~JavaScript
db.collection.createIndex( { orderTime: 1 }, { unique: true } )
~~~
2. 删除索引
索引无法修改，只能删除重建
~~~JavaScript
db.collection.dropIndex(index)
~~~
可以通过indexname删除
~~~
db.pets.dropIndex( "catIdx" )
~~~
或者index json删除
~~~
db.pets.dropIndex( { "cat" : -1 } )
~~~

## 查询进阶
## 数组查询
示例文档
~~~JavaScript
{ _id: 5, type: "food", item: "aaa", ratings: [ 5, 8, 9 ] }
{ _id: 6, type: "food", item: "bbb", ratings: [ 5, 9 ] }
{ _id: 7, type: "food", item: "ccc", ratings: [ 9, 5, 8 ] }
~~~

1. 完全匹配
~~~JavaScript
db.inventory.find( { ratings: [ 5, 8, 9 ] } )
~~~
返回结果:
~~~JavaScript
{ "_id" : 5, "type" : "food", "item" : "aaa", "ratings" : [ 5, 8, 9 ] }
~~~

2. 匹配数组元素
~~~JavaScript
db.inventory.find( { ratings: 5 } )
~~~
返回结果:
~~~JavaScript
{ "_id" : 5, "type" : "food", "item" : "aaa", "ratings" : [ 5, 8, 9 ] }
{ "_id" : 6, "type" : "food", "item" : "bbb", "ratings" : [ 5, 9 ] }
{ "_id" : 7, "type" : "food", "item" : "ccc", "ratings" : [ 9, 5, 8 ] }
~~~

3. 匹配数组指定元素
~~~JavaScript
db.inventory.find( { 'ratings.0': 5 } )
~~~
返回结果:
~~~JavaScript
{ "_id" : 5, "type" : "food", "item" : "aaa", "ratings" : [ 5, 8, 9 ] }
{ "_id" : 6, "type" : "food", "item" : "bbb", "ratings" : [ 5, 9 ] }
~~~

4. 数组元素多条件查询
a. $elemMatch:至少一个数组元素符合所有条件
~~~JavaScript
db.inventory.find( { ratings: { $elemMatch: { $gt: 5, $lt: 9 } } } )
~~~
查询结果：
~~~JavaScript
{ "_id" : 5, "type" : "food", "item" : "aaa", "ratings" : [ 5, 8, 9 ] }
{ "_id" : 7, "type" : "food", "item" : "ccc", "ratings" : [ 9, 5, 8 ] }
~~~
b. 一个元素各符合一个条件，或者单个元素符合所有条件
~~~JavaScript
db.inventory.find( { ratings: { $gt: 5, $lt: 9 } } )
~~~
查询结果：
~~~JavaScript
{ "_id" : 5, "type" : "food", "item" : "aaa", "ratings" : [ 5, 8, 9 ] }
{ "_id" : 6, "type" : "food", "item" : "bbb", "ratings" : [ 5, 9 ] }
{ "_id" : 7, "type" : "food", "item" : "ccc", "ratings" : [ 9, 5, 8 ] }
~~~
`{_id:6}`的document符合5<9和9>5的条件，所以返回


## 统计查询简介
此处统计仅仅给出一个示例供参考，并未详细阐述，有需要可以查询官方文档。
1. aggregate函数
~~~JavaScript
db.collection.aggregate(pipeline, options)
~~~
示例：
以cust_id分组，对status='A'的document求和，并倒序排列
~~~JavaScript
db.orders.aggregate([
                     { $match: { status: "A" } },
                     { $group: { _id: "$cust_id", total: { $sum: "$amount" } } },
                     { $sort: { total: -1 } }
                   ])
~~~
2. group函数
~~~JavaScript
db.collection.group({ key, reduce, initial [, keyf] [, cond] [, finalize] })
~~~
示例：
~~~
{ "ord_dt" : ISODate("2012-07-01T04:00:00Z"), "item.sku" : "abc123"}
{ "ord_dt" : ISODate("2012-07-01T04:00:00Z"), "item.sku" : "abc456"}
~~~
统计、求和、平均值
~~~JavaScript
db.orders.group(
   {
     keyf: function(doc) {
               return { day_of_week: doc.ord_dt.getDay() };
           },
     cond: { ord_dt: { $gt: new Date( '01/01/2012' ) } },
    reduce: function( curr, result ) {
                result.total += curr.item.qty;
                result.count++;
            },
    initial: { total : 0, count: 0 },
    finalize: function(result) {
                  var weekdays = [
                       "Sunday", "Monday", "Tuesday",
                       "Wednesday", "Thursday",
                       "Friday", "Saturday"
                      ];
                  result.day_of_week = weekdays[result.day_of_week];
                  result.avg = Math.round(result.total / result.count);
              }
   }
)
~~~
3. map/reduce函数
aggreate函数和group函数均受到32M大小的限制，map/reduce不受此限制
~~~JavaScript
db.collection.mapReduce(
                         <map>,
                         <reduce>,
                         {
                           out: <collection>,
                           query: <document>,
                           sort: <document>,
                           limit: <number>,
                           finalize: <function>,
                           scope: <document>,
                           jsMode: <boolean>,
                           verbose: <boolean>
                         }
                       )
~~~
示例：
~~~JavaScript
{
     _id: ObjectId("50a8240b927d5d8b5891743c"),
     cust_id: "abc123",
     ord_date: new Date("Oct 04, 2012"),
     status: 'A',
     price: 25,
     items: [ { sku: "mmm", qty: 5, price: 2.5 },
              { sku: "nnn", qty: 5, price: 2.5 } ]
}
~~~
按`item.sku`分组，统计每种sku个数，并计算该sku的quantity，最终求取平均值
map function
~~~JavaScript
var mapFunction2 = function() {
                       for (var idx = 0; idx < this.items.length; idx++) {
                           var key = this.items[idx].sku;
                           var value = {
                                         count: 1,
                                         qty: this.items[idx].qty
                                       };
                           emit(key, value);
                       }
                    };
~~~
reduce function
~~~JavaScript
var reduceFunction2 = function(keySKU, countObjVals) {
                     reducedVal = { count: 0, qty: 0 };

                     for (var idx = 0; idx < countObjVals.length; idx++) {
                         reducedVal.count += countObjVals[idx].count;
                         reducedVal.qty += countObjVals[idx].qty;
                     }

                     return reducedVal;
                  };
~~~
finalize function
~~~JavaScript
var finalizeFunction2 = function (key, reducedVal) {

                       reducedVal.avg = reducedVal.qty/reducedVal.count;

                       return reducedVal;

                    };
~~~
map/reduce execute
~~~JavaScript
db.orders.mapReduce( mapFunction2,
                     reduceFunction2,
                     {
                       out: { merge: "map_reduce_example" },
                       query: { ord_date:
                                  { $gt: new Date('01/01/2012') }
                              },
                       finalize: finalizeFunction2
                     }
                   )
~~~

## MongoDB CURD优化分析
1. explain分析
执行：`db.collection.find(<query>).explain()`
输出：[输出分析](http://docs.mongodb.org/manual/reference/explain-results/ "mogodb explain result")
2. 慢查询
~~~JavaScript
db.setProfilingLevel(level, slowms)
~~~
level: 0 不记录 1 记录时间超过slowms的操作 2 记录所有 
3. killop
通过`db.killOp(opid)`杀死慢查询,其中opid可以通过`db.currentOp()`获得
 
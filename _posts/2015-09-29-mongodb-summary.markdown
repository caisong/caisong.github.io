---
layout: post
title:  "mongod summary!"
date:   2015-09-29 15:09:00
categories: mongodb
---
* ###复制集配置  
  1. 启动mongd实例  
  {% highlight JavaScript %}
  mongod --replicaSet <setname> --port <port> --dbpath <dbpath>  --logpath <logpath> --directoryperdb
  {% endhighlight %}
  2. 配置复制集  
  {% highlight JavaScript %}
  rs.initiate(
      {
          _id : <setname>,
          members : 
          [
              {_id : 0, host : <host0>},
              {_id : 1, host : <host1>},
              {_id : 2, host : <host2>},
          ]
      })
  {% endhighlight %}
  3. 校验   
  {% highlight JavaScript %}
    
    rs.status()
    rs.isMaster()
  {% endhighlight %}
  4. 其他  
  以下操作都必须在primary节点上进行  
  {% highlight JavaScript %}
    //添加普通节点  
    rs.add(<ip:port>)
    //添加仲裁节点   
    rs.add(<ip:port>,true)
    rs.addArb(<ip:port>)
    //移除节点   
    rs.remove(<ip:port>)
  {% endhighlight %}
* ###单实例转复制集
  1. stop mongod
  2. 启动命令中添加{% highlight JavaScript %}--replicaSet <setname>{% endhighlight %} 参数
  3. 重启启动mongod
  {% highlight JavaScript %}
    rs.initiate()
    rs.add(<hostname>)
  {% endhighlight %}
  
* ###shard配置
  1. 启动mongd实例  
   {% highlight JavaScript %}
        //config 节点
        mongod --configsvr --port <port> --dbpath <dbpath>  --logpath <logpath> --directoryperdb
        //mongos 节点 
        //<ip:port> 是config节点的ip和port
        mongos --port <port> --logpath <logpath> --configdb <ip:port>
        //shard 节点
        mongod --shardsvr --port <port> --dbpath <dbpath>  --logpath <logpath> --directoryperdb
   {% endhighlight %}
  2. 添加shard
   {% highlight JavaScript %}
       //连接mongos
       mongo <ip:port>
       //添加shard
       sh.addShard(<ip:port>)
   {% endhighlight %}
  3. 配置shard参数  
    a. 修改chunkSize大小
    {% highlight JavaScript %}
        use config
        db.settings.save({ "_id" : "chunksize", "value" : <newsize>})
    {% endhighlight %}
    b. Balancer设置
    {% highlight JavaScript %}
        //关闭Balancer
        sh.stopBalancer()
        //获取Balancer状态
        sh.getBalancerState()
        //开启Balancer
        sh.startBalancer()
    {% endhighlight %}
  4. 数据库分片
    {% highlight JavaScript %}
        //1. 允许分片
        sh.enableSharding(<dbname>)
        //2. 设置片键
        sh.shardCollection(<namespace>,{<keyname>:1})
     {% endhighlight %}
    片键可以设置为hash片键,如 {% highlight JavaScript %}{_id:"hahsed"}{% endhighlight %}
  5. 其他
    a. 手动划分切片
    {% highlight JavaScript %}
        sh.splitAt(<namespace>, <query>)
    {% endhighlight %}
    b. 手动移动切片
    {% highlight JavaScript %}
        sh.moveChunk(<namespace>, <query>, <destination>})
    {% endhighlight %}  
        <namespace> 指dbbase.collection  
        <destination> 指shardname 可以通过{% highlight JavaScript %}shard.status{% endhighlight %}查看 
    c. 移除shard
    {% highlight JavaScript %}
        use admin
        db.runCommand( { removeShard : <shardname> } )
    {% endhighlight %} 
    d. 添加复制集到shard
    {% highlight JavaScript %}
        //至少需要添加复制集的一个成员节点  
        sh.addShard( "rs1/mongodb0.example.net:27017")
    {% endhighlight %}
    如果已有Shard中存在同名的数据库，则添加会失败；其他分片、切片操作与单机相同
    
* ###常用JavaScript配置示例
* 复制集初始化
{% highlight JavaScript %}
  rs.initiate(
      {
          _id : rs91,
          members : 
          [
              {_id : 0, host :"192.168.10.91:25521"},
              {_id : 1, host :"192.168.10.91:25522"},
              {_id : 2, host :"192.168.10.91:25523"}
          ]
      })
{% endhighlight %}
* shard初始化  
{% highlight JavaScript %}
    use config  
    db.settings.save({ "_id" : "chunksize", "value" : 1024})
    sh.stopBalancer()
    sh.enableSharding("test")
    sh.shardCollection("test.collection",{_id:1})
    sh.splitAt("test.collection",{_id:100})
    sh.moveChunk("test.collection",{_id:{$gte:100}},"shard0000")
{% endhighlight %}
* group 函数示例  
数据结构
{% highlight JavaScript %}
    {
    _id: ObjectId("5085a95c8fada716c89d0021"),
    ord_dt: ISODate("2012-07-01T04:00:00Z"),
    ship_dt: ISODate("2012-07-02T04:00:00Z"),
    item: { sku: "abc123",
            price: 1.99,
            uom: "pcs",
            qty: 25 }
    }
{% endhighlight %}
group Sum, Count, and Average
{% highlight JavaScript %}
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
{% endhighlight %}
* mapReduce函数示例
数据示例
{% highlight JavaScript %}
    {
        _id: ObjectId("50a8240b927d5d8b5891743c"),
        cust_id: "abc123",
        ord_date: new Date("Oct 04, 2012"),
        status: 'A',
        price: 25,
        items: [ { sku: "mmm", qty: 5, price: 2.5 },
                { sku: "nnn", qty: 5, price: 2.5 } ]
    }
{% endhighlight %}
mapReduce函数
  * map函数
  {% highlight JavaScript %}
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
  {% endhighlight %}
  * reduce函数
  {% highlight JavaScript %}
    var reduceFunction2 = function(keySKU, countObjVals) {
                        reducedVal = { count: 0, qty: 0 };
    
                        for (var idx = 0; idx < countObjVals.length; idx++) {
                            reducedVal.count += countObjVals[idx].count;
                            reducedVal.qty += countObjVals[idx].qty;
                        }
    
                        return reducedVal;
                    };
  {% endhighlight %}
  * finalize函数
  {% highlight JavaScript %}
    var finalizeFunction2 = function (key, reducedVal) {
    
                        reducedVal.avg = reducedVal.qty/reducedVal.count;
    
                        return reducedVal;
    
                        };
  {% endhighlight %}
  * 执行
  {% highlight JavaScript %}
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
  {% endhighlight %}

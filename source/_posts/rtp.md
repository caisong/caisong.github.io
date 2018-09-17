---
title: rtp协议
date: 2018-09-17 21:51:27
tags: rtp,c++
---

* 部分内容转载自：[雷霄骅CSDN博客](https://blog.csdn.net/leixiaohua1020/article/details/50535230 "leixiaohua")

## 固定头部格式
```plain
		1               2               3              4
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|V=2|P|X|  CC   |M|     PT      |       sequence number         |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                           timestamp                           |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                synchronization source (SSRC)                  |   
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                 contributing source (CSRC)                    | 
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

* V：RTP协议的版本号，占2位，当前协议版本号为2
* P：填充标志，占1位，如果P=1，则在该报文的尾部填充一个或多个额外的八位组，它们不是有效载荷的一部分。
* X：扩展标志，占1位，如果X=1，则在RTP报头后跟有一个扩展报头
* CC：CSRC计数器，占4位，指示CSRC 标识符的个数
* M: 标记，占1位，不同的有效载荷有不同的含义，对于视频，标记一帧的结束；对于音频，标记会话的开始。
* PT: 有效荷载类型，占7位，用于说明RTP报文中有效载荷的类型，如GSM音频、JPEG图像等,在流媒体中大部分是用来区分音频流和视频流的，这样便于客户端进行解析。
* 序列号：占16位，用于标识发送者所发送的RTP报文的序列号，每发送一个报文，序列号增1。这个字段当下层的承载协议用UDP的时候，网络状况不好的时候可以用来检查丢包。
同时出现网络抖动的情况可以用来对数据进行重新排序，序列号的初始值是随机的，同时音频包和视频包的sequence是分别记数的。
* 时戳(Timestamp)：占32位，必须使用90 kHz 时钟频率。时戳反映了该RTP报文的第一个八位组的采样时刻。接收者使用时戳来计算延迟和延迟抖动，并进行同步控制。
* 同步信源(SSRC)标识符：占32位，用于标识同步信源。该标识符是随机选择的，参加同一视频会议的两个同步信源不能有相同的SSRC。
* 特约信源(CSRC)标识符：每个CSRC标识符占32位，可以有0～15个。每个CSRC标识了包含在该RTP报文有效载荷中的所有特约信源。

## 固定头部解析

```cpp
typedef struct _rtp_fixed_header{
	/* byte 0 */
	unsigned char csrc_len:4;       /* expect 0 */
	unsigned char extension:1;      /* expect 1 */
	unsigned char padding:1;        /* expect 0 */
	unsigned char version:2;        /* expect 2 */
	/* byte 1 */
	unsigned char payload:7;
	unsigned char marker:1;        /* expect 1 */
	/* bytes 2, 3 */
	unsigned short seq_no;            
	/* bytes 4-7 */
	unsigned  int timestamp;        
	/* bytes 8-11 */
	unsigned int ssrc;            /* stream number is used here. */
} RTP_FIXED_HEADER;
```

### 补充

* 声明后加长度，表明占用的bit, 在协议解析中常用
* 如果出现前面字段比后面字段长的情况，需要注意字节对齐的情况。

```cpp
	typedef struct _some_protocol{
		unsigned short field_A;
		unsigned char len;
		unsigned short flag_B;
	}SOME_PROTOCOL ;
```

此时`sizeof SOME_PROTOOCL`的长度是6，没法直接将字节流转为对应的结构体，需要指定对齐长度

```cpp
#pragma pack(1)
/**/
#pragma pack()
```


## 扩展头部
```plain
	1               2               3              4
	0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Defined by profile        |             length            |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    header extension                           |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

当RTP头部中固定头部的 X 为1时， 包括一个扩展头部.紧接着CSRC（如果存在）.
* 前16bit 用以识别标识符或参数，由上层协议定义
* length 指示 header extension 的长度，**单位32bit**


## wireshark识别

* 解码
  菜单栏依次选择分析、解码为，弹出菜单右下角添加，装着RTP，应用。

* 流分析
菜单栏依次选择：电话、RTP、流分析。


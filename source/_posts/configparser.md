---
title: python发送邮件（续）—— configparser
date: 2016-06-03 20:45:12
tags:
  - mail
  - config
  - ini
categories: python
---
## 简介
昨天代码一些基本配置全部在代码中，灵活性较差，考虑到通过配置文件的方式来实现。结构比较简单，不想用xml或json，类似配置数据库连接的`property`文件就可以了，`configparser`比较满足要求。
python `configparser`能够读取`*.conf`与`*.ini`的配置文件
<!-- more -->
以下内容parser摘录自：[configparserExamples 示例](https://wiki.python.org/moin/ConfigParserExamples "configparserExamples")

## configparser 使用
### 读配置文件
```ini
[SectionOne]
Status: Single
Name: Derek
Value: Yes
Age: 30
Single: True

[SectionTwo]
FavoriteColor = Green, Red, Blue
[SectionThree]
FamilyName: Johnson

[Others]
Route: 66
```

```python
import ConfigParser
config = ConfigParser.ConfigParser()
config.read('config file path')
# read node
config.get('SectionOne','Status)
# read node as list
config.get('SectionTwo','FavoriteColor').replace(' ','').split(',')
```
### 写配置文件
```python
# lets create that config file for next time...
cfgfile = open("c:\\next.ini",'w')

# add the settings to the structure of the file, and lets write it out...
Config.add_section('Person')
Config.set('Person','HasEyes',True)
Config.set('Person','Age', 50)
Config.write(cfgfile)
cfgfile.close()
```

### 读取含有宏的配置文件
```ini
[SectionOne]
Param1: Hello
Param2: World

[SectionTwo]
Param1: ${SectionOne:Param1} ${SectionOne:Param2}

[SectionThree]
Alpha: One
Bravo: Two
Charlie: ${Alpha} Mississippi
```

```python
>>> import configparser
>>> settings = configparser.ConfigParser()
>>> settings._interpolation = configparser.ExtendedInterpolation()
>>> settings.read('settings.ini')
['settings.ini']
>>> settings.sections()
['SectionOne', 'SectionTwo', 'SectionThree']
>>> settings.get('SectionTwo', 'Param1')
'Hello World'
>>> settings.get('SectionThree', 'Charlie')
'One Mississippi'
```

## 发送邮件代码
```ini
[mail]
address: abc@qq.com
password: XXXXXXXXXX
smtp: smtp.qq.com
port: 465
subject: XXX 工作日志
reAddr: abc@qq.com, bcd@qq.com
ccAddr: abc@qq.com, bcd@qq.com
contextPath: a.txt
attachPath: b.txt 
```

```python
# -*- coding: utf-8 -*-
'''
发送带附件邮件
'''

from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import smtplib
from datetime import date
import configparser

# 读取配置
config = configparser.ConfigParser()
config.read('mail.ini', encoding='utf-8')
address = config.get('mail', 'address')
# 可以考虑加密
password = config.get('mail', 'password')
smtp = config.get('mail', 'smtp')
port = config.get('mail', 'port')
subject = config.get('mail', 'subject')
# 收件人
receiver = config.get('mail', 'reAddr')
# 抄送人
carbonCopy = config.get('mail', 'ccAddr')
# 正文
context = config.get('mail', 'contextPath')
# 附件(暂时考虑的一个)
attachment = config.get('mail', 'attachPath')

# 创建一个带附件的实例
msg = MIMEMultipart()

# 加邮件头
msg['from'] = address
msg['to'] = receiver
msg['subject'] = date.today().strftime("%Y-%m-%d") + subject
msg['cc'] = carbonCopy
msg.attach(MIMEText(open(context, 'rb').read(), 'plain', 'utf-8'))

# 构造附件
with open(attachment, 'rb') as myfile:
    att = MIMEText(myfile.read(), 'base64', 'utf-8')
    att["Content-Type"] = 'application/octet-stream'
    # 附件名
    filename = attachment.split('\\')[-1]
    att["Content-Disposition"] = 'attachment; filename=' + filename
    msg.attach(att)
# 发送邮件
try:
    server = smtplib.SMTP_SSL(smtp, int(port))
    server.login(address, password)
    # 所有收件人（包括抄送）
    receiverList = (receiver + ',' + carbonCopy).replace(' ', '').split(',')
    server.sendmail(msg['from'], receiverList, msg.as_string())
    server.quit()
    print('''success''')
except Exception as e:
    print(e)
```

## 注意事项
1. 若密码中包含特殊字符，需要转义
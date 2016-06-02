---
title: python 发送邮件
date: 2016-06-02 20:46:58
tags: 
  - email
  - python
categories: python
---

## 需求
每天发送工作日报，需要send一堆人，cc一堆人（公司规定，呵呵），还要添加附件。考虑到python能够方便的发送邮箱，就打算写个脚本。
<!-- more -->

## 代码
```python

# -*- coding: utf-8 -*-
#!/usr/bin/env python3
'''
发送带附件邮件
'''

from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import smtplib
from datetime import date

# 创建一个带附件的实例
msg = MIMEMultipart()

# 接收列表
receiverList = ['abc@yeah.net', 'abc@qq.com']
# 抄送列表
ccList = ['abc@yeah.net', 'abc@qq.com']

# 邮件头
msg['from'] = 'song_cai@yeah.net'
msg['to'] = ', '.join(receiverList)
msg['subject'] = date.today().strftime("%Y-%m-%d") + '''XX工作日报'''
msg['cc'] = ', '.join(ccList)

# 正文
msg.attach(MIMEText(open('d:\\mailContext.txt',
                         'rb').read(), 'plain', 'utf-8'))

# 附件
att = MIMEText(open('d:\\mailContext.txt', 'rb').read(), 'base64', 'utf-8')
att["Content-Type"] = 'application/octet-stream'
att["Content-Disposition"] = 'attachment; filename="mailContext.txt"'
msg.attach(att)

# 发送邮件
try:
    server = smtplib.SMTP_SSL('''smtp.qq.com''', 465)
    server.login('song_cai@yeah.net', '''xxxxx''')
    server.sendmail(msg['from'], receiverList + ccList, msg.as_string())
    server.quit()
    print('''success''')
except Exception as e:
    input(e)
```

## 注意事项
* 现在大部分邮箱使用SSL方式，所以对应地必须使用`SMTP_SSL`,直接指定地址和端口，使用`connect`的方式指定存在问题
* 正文要写在附件之前，否则会出现两个附件（即正文部分也成为附件了）

** 懒惰使人进步 **


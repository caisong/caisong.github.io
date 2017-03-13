---
title: set_ip_addr_on_boot
date: 2017-03-13 22:29:19
tags: shell, update-rc.d, systemctl
category: linux
---

## update-rc.d

* the script (setipaddr.sh)
```bash
#! /bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/bin:$PATH
ifconfig eth0 192.168.10.1/24
```

* update-rc.d setting
```shell
chmod +x setipaddr.sh
mv setipaddr.sh /etc/init.d
update-rc.d setipaddr.sh defaults
```

* run level
```bash
# If defaults is used then update-rc.d will make links to start the service in runlevels 2345 and to stop the service in runlevels 016
runlevel # get current level
```

## systemctl
* service file (/etc/systemd/system/setipaddr.service)  
```bash
[Unit]
Description=set ip on boot
After=network.target

[Service]
Type=oneshot
ExecStart=/opt/bin/setipaddr.sh

[Install]
WantedBy=multi-user.target
```

* systemctl setting
```bash
systemctl enable setipaddr.service
```



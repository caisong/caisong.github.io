---
title: xinetd 配置
date: 2017-06-07 21:39:31
tags: xinetd
---

0. 介绍  

> xinetd - the extended Internet services daemon
> xinetd  performs  the  same  function as inetd: it starts programs that
provide Internet services.  Instead of having such servers  started  at
system  initialization  time, and be dormant until a connection request
arrives, xinetd is the only daemon process started and  it  listens  on
all  service  ports  for the services listed in its configuration file.
When a request comes in, xinetd starts the appropriate server.  Because
of  the  way it operates, xinetd (as well as inetd) is also referred to
as a super-server.
>
> The services listed in xinetd’s configuration  file  can  be  separated
into two groups.  Services in the first group are called multi-threaded
and they require the forking of a new server process for each new  con-
nection  request.   The  new  server then handles that connection.  For
such services, xinetd keeps listening for new requests so that  it  can
spawn  new  servers.  On the other hand, the second group includes ser-
vices for which the service daemon is responsible for handling all  new
connection  requests.   Such  services  are  called single-threaded and
xinetd will stop handling new requests for them until the server  dies.
Services in this group are usually datagram-based.
>
> So far, the only reason for the existence of a super-server was to con-
serve system resources by avoiding to fork a  lot  of  processes  which
might  be  dormant  for  most of their lifetime.  While fulfilling this
function, xinetd takes advantage of the idea of a super-server to  pro-
vide  features such as access control and logging.  Furthermore, xinetd
is not limited to services listed in /etc/services.  Therefore, anybody

以下内容转载自 [linuxfromscratch](http://www.linuxfromscratch.org/blfs/view/6.3/server/xinetd.html "xinetd")  

0. xinetd.conf文件    

```shell
cat > /etc/xinetd.conf << "EOF"
# Begin /etc/xinetd
# Configuration file for xinetd
#

defaults
{
      instances       = 60
      log_type        = SYSLOG daemon
      log_on_success  = HOST PID USERID
      log_on_failure  = HOST USERID
      cps             = 25 30
}

# All service files are stored in the /etc/xinetd.d directory
#
includedir /etc/xinetd.d
# End /etc/xinetd
EOF
```   

0. 常见服务项配置    

```shell
install -v -d -m755 /etc/xinetd.d &&
cat > /etc/xinetd.d/login << "EOF" &&
# Begin /etc/xinetd.d/login

service login
{
   disable        = yes
   socket_type    = stream
   protocol       = tcp
   wait           = no
   user           = root
   server         = /usr/sbin/in.rlogind
   log_type       = SYSLOG local4 info
}

# End /etc/xinetd.d/login
EOF
cat > /etc/xinetd.d/shell << "EOF" &&
# Begin /etc/xinetd.d/shell

service shell
{
   disable        = yes
   socket_type    = stream
   wait           = no
   user           = root
   instances      = UNLIMITED
   flags          = IDONLY
   log_on_success += USERID
   server         = /usr/sbin/in.rshd
}

# End /etc/xinetd.d/shell
EOF
cat > /etc/xinetd.d/exec << "EOF" &&
# Begin /etc/xinetd.d/exec

service exec
{
   disable        = yes
   socket_type    = stream
   wait           = no
   user           = root
   server         = /usr/sbin/in.rexecd
}

# End /etc/xinetd.d/exec
EOF
cat > /etc/xinetd.d/comsat << "EOF" &&
# Begin /etc/xinetd.d/comsat

service comsat
{
   disable        = yes
   socket_type    = dgram
   wait           = yes
   user           = nobody
   group          = tty
   server         = /usr/sbin/in.comsat
}

# End /etc/xinetd.d/comsat
EOF
cat > /etc/xinetd.d/talk << "EOF" &&
# Begin /etc/xinetd.d/talk

service talk
{
   disable        = yes
   socket_type    = dgram
   wait           = yes
   user           = root
   server         = /usr/sbin/in.talkd
}

# End /etc/xinetd.d/talk
EOF
cat > /etc/xinetd.d/ntalk << "EOF" &&
# Begin /etc/xinetd.d/ntalk

service ntalk
{
   disable        = yes
   socket_type    = dgram
   wait           = yes
   user           = root
   server         = /usr/sbin/in.ntalkd
}

# End /etc/xinetd.d/ntalk
EOF
cat > /etc/xinetd.d/telnet << "EOF" &&
# Begin /etc/xinetd.d/telnet

service telnet
{
   disable        = yes
   socket_type    = stream
   wait           = no
   user           = root
   server         = /usr/sbin/in.telnetd
   bind           = 127.0.0.1
   log_on_failure += USERID
}

service telnet
{
   disable        = yes
   socket_type    = stream
   wait           = no
   user           = root
#  server         = /usr/sbin/in.telnetd
   bind           = 192.231.139.175
   redirect       = 128.138.202.20 23
   log_on_failure += USERID
}

# End /etc/xinetd.d/telnet
EOF
cat > /etc/xinetd.d/ftp << "EOF" &&
# Begin /etc/xinetd.d/ftp

service ftp
{
   disable        = yes
   socket_type    = stream
   wait           = no
   user           = root
   server         = /usr/sbin/in.ftpd
   server_args    = -l
   instances      = 4
   log_on_success += DURATION USERID
   log_on_failure += USERID
   access_times   = 2:00-8:59 12:00-23:59
   nice           = 10
}

# End /etc/xinetd.d/ftp
EOF
cat > /etc/xinetd.d/tftp << "EOF" &&
# Begin /etc/xinetd.d/tftp

service tftp
{
   disable        = yes
   socket_type    = dgram
   wait           = yes
   user           = root
   server         = /usr/sbin/in.tftpd
   server_args    = -s /tftpboot
}

# End /etc/xinetd.d/tftp
EOF
cat > /etc/xinetd.d/finger << "EOF" &&
# Begin /etc/xinetd.d/finger

service finger
{
   disable        = yes
   socket_type    = stream
   wait           = no
   user           = nobody
   server         = /usr/sbin/in.fingerd
}

# End /etc/xinetd.d/finger
EOF
cat > /etc/xinetd.d/systat << "EOF" &&
# Begin /etc/xinetd.d/systat

service systat
{
   disable           = yes
   socket_type       = stream
   wait              = no
   user              = nobody
   server            = /usr/bin/ps
   server_args       = -auwwx
   only_from         = 128.138.209.0
   log_on_success    = HOST
}

# End /etc/xinetd.d/systat
EOF
cat > /etc/xinetd.d/netstat << "EOF" &&
# Begin /etc/xinetd.d/netstat

service netstat
{
   disable           = yes
   socket_type       = stream
   wait              = no
   user              = nobody
   server            = /usr/ucb/netstat
   server_args       = -f inet
   only_from         = 128.138.209.0
   log_on_success    = HOST
}

# End /etc/xinetd.d/netstat
EOF
cat > /etc/xinetd.d/echo << "EOF" &&
# Begin /etc/xinetd.d/echo

service echo
{
   disable     = yes
   type        = INTERNAL
   id          = echo-stream
   socket_type = stream
   protocol    = tcp
   user        = root
   wait        = no
}

service echo
{
   disable     = yes
   type        = INTERNAL
   id          = echo-dgram
   socket_type = dgram
   protocol    = udp
   user        = root
   wait        = yes
}

# End /etc/xinetd.d/echo
EOF
cat > /etc/xinetd.d/chargen << "EOF" &&
# Begin /etc/xinetd.d/chargen

service chargen
{
   disable        = yes
   type           = INTERNAL
   id             = chargen-stream
   socket_type    = stream
   protocol       = tcp
   user           = root
   wait           = no
}

service chargen
{
   disable        = yes
   type           = INTERNAL
   id             = chargen-dgram
   socket_type    = dgram
   protocol       = udp
   user           = root
   wait           = yes
}

# End /etc/xinetd.d/chargen
EOF
cat > /etc/xinetd.d/daytime << "EOF" &&
# Begin /etc/xinetd.d/daytime

service daytime
{
   disable        = yes
   type           = INTERNAL
   id             = daytime-stream
   socket_type    = stream
   protocol       = tcp
   user           = root
   wait           = no
}

service daytime
{
   disable        = yes
   type           = INTERNAL
   id             = daytime-dgram
   socket_type    = dgram
   protocol       = udp
   user           = root
   wait           = yes
}

# End /etc/xinetd.d/daytime
EOF
cat > /etc/xinetd.d/time << "EOF" &&
# Begin /etc/xinetd.d/time

service time
{
   disable        = yes
   type           = INTERNAL
   id             = time-stream
   socket_type    = stream
   protocol       = tcp
   user           = root
   wait           = no
}


service time
{
   disable        = yes
   type           = INTERNAL
   id             = time-dgram
   socket_type    = dgram
   protocol       = udp
   user           = root
   wait           = yes
}

# End /etc/xinetd.d/time
EOF
cat > /etc/xinetd.d/rstatd << "EOF" &&
# Begin /etc/xinetd.d/rstatd

service rstatd
{
   disable     = yes
   type        = RPC
   flags       = INTERCEPT
   rpc_version = 2-4
   socket_type = dgram
   protocol    = udp
   server      = /usr/sbin/rpc.rstatd
   wait        = yes
   user        = root
}

# End /etc/xinetd.d/rstatd
EOF
cat > /etc/xinetd.d/rquotad << "EOF" &&
# Begin /etc/xinetd.d/rquotad

service rquotad
{
   disable     = yes
   type        = RPC
   rpc_version = 1
   socket_type = dgram
   protocol    = udp
   wait        = yes
   user        = root
   server      = /usr/sbin/rpc.rstatd
}

# End /etc/xinetd.d/rquotad
EOF
cat > /etc/xinetd.d/rusersd << "EOF" &&
# Begin /etc/xinetd.d/rusersd

service rusersd
{
   disable     = yes
   type        = RPC
   rpc_version = 1-2
   socket_type = dgram
   protocol    = udp
   wait        = yes
   user        = root
   server      = /usr/sbin/rpc.rusersd
}

# End /etc/xinetd.d/rusersd
EOF
cat > /etc/xinetd.d/sprayd << "EOF" &&
# Begin /etc/xinetd.d/sprayd

service sprayd
{
   disable      = yes
   type         = RPC
   rpc_version  = 1
   socket_type  = dgram
   protocol     = udp
   wait         = yes
   user         = root
   server       = /usr/sbin/rpc.sprayd
}

# End /etc/xinetd.d/sprayd
EOF
cat > /etc/xinetd.d/walld << "EOF" &&
# Begin /etc/xinetd.d/walld

service walld
{
   disable      = yes
   type         = RPC
   rpc_version  = 1
   socket_type  = dgram
   protocol     = udp
   wait         = yes
   user         = nobody
   group        = tty
   server       = /usr/sbin/rpc.rwalld
}

# End /etc/xinetd.d/walld
EOF
cat > /etc/xinetd.d/irc << "EOF"
# Begin /etc/xinetd.d/irc

service irc
{
   disable      = yes
   socket_type  = stream
   wait         = no
   user         = root
   flags        = SENSOR
   type         = INTERNAL
   bind         = 192.168.1.30
   deny_time    = 60
}

# End /etc/xinetd.d/irc
EOF
```

3. 使用方式  
若要激活服务，将配置文件中的`disable = yes`替换为`disable  = no`
重新启动`xinetd.service`

4. 权限问题
  1. `echo rsh >>/etc/securetty`
  2. 注释/etc/pam.d/目录下服务授权配置（待展开）

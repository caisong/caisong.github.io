---
title: gdb problem inside docker container
date: 2017-12-17 20:53:59
tags: 
  - docker
  - gdb
categories: docker
---

> warning: Error disabling address space randomization: Operation not permitted

* solutions:[stackoverflow](https://stackoverflow.com/questions/35860527/warning-error-disabling-address-space-randomization-operation-not-permitted)

  ```bash
  docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined
  ```

  ** There is no such options for `docker exec` command, so I recreate a container. Maybe there are others ways, please let me know. Thanks **



---
title: Git实用命令
date: 2021-09-03T21:42:00+08:00
lastmod: 2021-09-03T21:42:00+08:00
author: Cai Song
# avatar: /img/author.jpg
# authorlink: https://author.site
cover: /img/cover.jpg
# images:
#   - /img/cover.jpg
categories:
  - git
tags:
  - git
# nolastmod: true
draft: false
---

[git 实用命令](https://github.com/uolcano/blog/issues/12)

git虽好，但总会遇到一些不希望的提交，所以就会有增删改某次或某些提交的需求。下面收集一下，修改本地和远程版本历史的一些方法。
## 本地修改

由于以下修改本身是对版本历史的修改，在需要push到远程仓库时，往往是不成功的，只能强行push，这样会出现的一个问题就是，如果你是push到多人协作的远程仓库中，会对其他人的远程操作构成影响。通常情况下，建议与项目远程仓库的管理员进行沟通，在完成你强制push操作后，通知其他人同步。

    修改最近一次的commit
    
    修改提交的描述
    
    git commit --amend
    
    然后会进入一个文本编辑器界面，修改commit的描述内容，即可完成操作。
    
    修改提交的文件
    
    git add <filename> # 或者 git rm
    git commit --amend # 将缓存区的内容做为最近一次提交
    
    修改任意提交历史位置的commit
    
    可以通过变基命令，修改最近一次提交以前的某次提交。不过修改的提交到当前提交之间的所有提交的hash值都会改变。
    变基操作需要非常小心，一定要多用git status命令来查看你是否还处于变基操作，可能某次误操作的会对后面的提交历史造成很大影响。
    
    首先查看提交日志，以便变基后，确认提交历史的修改
    
    git log
    
    变基操作。 可以用commit~n或commit^^这种形式替代：前者表示当前提交到n次以前的提交，后者^符号越多表示的范围越大，commit可以是HEAD或者某次提交的hash值；-i参数表示进入交互模式。
    
    git rebase -i <commit range>
    
    以上变基命令会进入文本编辑器，其中每一行就是某次提交，把pick修改为edit，保存退出该文本编辑器。
    
    **注意：**变基命令打开的文本编辑器中的commit顺序跟git log查看的顺序是相反的，也就是最近的提交在下面，老旧的提交在上面
    
    **注意：**变基命令其实可以同时对多个提交进行修改，只需要修改将对应行前的pick都修改为edit，保存退出后会根据你修改的数目多次打开修改某次commit的文本编辑器界面。但是这个范围内的最终祖先commit不能修改，也就是如果有5行commit信息，你只能修改下面4行的，这不仅限于commit修改，重排、删除以及合并都如此。
    
    git commit --amend
    
    接下来修改提交描述内容或者文件内容，跟最近一次的commit的操作相同，不赘述。
    
    然后完成变基操作
    
    git rebase --continue
    
    有时候会完成变基失败，需要git add --all才能解决，一般git会给出提示。
    
    再次查看提交日志，对比变基前后的修改，可以看到的内的所有提交的hash值都被修改了
    
    git log
    
    如果过了一段时间后，你发现这次历史修改有误，想退回去怎么办？请往下继续阅读
    
    重排或删除某些提交
    
    变基命令非常强大，还可以将提交历史重新手动排序或者删除某次提交。这为某些误操作，导致不希望公开信息的提交，提供了补救措施
    
    git rebase -i <commit range>
    
    如前面描述，这会进入文本编辑器，对某行提交进行排序或者删除，保存退出。可以是多行修改。
    
    后续操作同上。
    
    合并多次提交
    
    非关键性的提交太多会让版本历史很难看、冗余，所以合并多次提交也是挺有必要的。同样是使用以上的变基命令，不同的是变基命令打开的文本编辑器里的内容的修改。
    
    将pick修改为squash，可以是多行修改，然后保存退出。这个操作会将标记为squash的所有提交，都合并到最近的一个祖先提交上。
    
    **注意：**不能对的第一行commit进行修改，至少保证第一行是接受合并的祖先提交。
    
    后续操作同上。
    
    分离某次提交
    
    变基命令还能分离提交，这里不描述，详情查看后面的参考链接
    
    终极手段
    
    git还提供了修改版本历史的“大杀器”——filter-branch，可以对整个版本历史中的每次提交进行修改，可用于删除误操作提交的密码等敏感信息。
    
    删除所有提交中的某个文件
    
    git filter-branch --treefilter 'rm -f password.txt' HEAD
    
    将新建的主目录作为所有提交的根目录
    
    git filter-branch --subdirectory-filter trunk HEAD

## 本地回退

回退操作也是对过往提交的一剂“后悔药”，常用的回退方式有三种：checkout、reset和revert

    checkout
    
    对单个文件进行回退。不会修改当前的HEAD指针的位置，也就是提交并未回退
    
    可以是某次提交的hash值，或者HEAD（缺省即默认）
    
    git checkout <commit> -- <filename>
    
    reset
    
    回退到某次提交。回退到的指定提交以后的提交都会从提交日志上消失
    **注意：**工作区和暂存区的内容都会被重置到指定提交的时候，如果不加--hard则只移动HEAD的指针，不影响工作区和暂存区的内容。
    
    git reset --hard <commit>
    
    结合git reflog找回提交日志上看不到的版本历史，撤回某次操作前的状态
    
    git reflog # 找到某次操作前的提交hash值
    git reset <commit>
    
    这个方法可以对你的回退操作进行回退，因为这时候git log命令已经找不到历史提交的hash值了。
    
    revert
    
    这个方法是最温和，最受推荐的，因为本质上不是修改过去的版本历史，而是将回退版本历史作为一次新的提交，所以不会改变版本历史，在push到远程仓库的时候也不会影响到团队其他人。
    
    git revert <commit>

## 远程修改

对远程仓库的版本历史修改，都是在本地修改的基础上进行的：本地修改完成后，再push到远程仓库。

但是除了git revert可以直接push，其他都会对原有的版本历史修改，只能使用强制push

```shell
git push -f <remote> <branch>
```

## 总结

* git commit --amend改写单次commit
* git rebase -i <commit range>删改排以及合并多个commit
* git checkout <commit> -- <filename>获取历史版本的某个文件
* git reset [--hard] <commit>移动HEAD指针
* git revert <commit>创建一个回退提交
* git push -f <remote> <branch>强制push，覆盖原有远程仓库


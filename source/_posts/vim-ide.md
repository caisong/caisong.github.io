---
title: 自定义语言IDE构建
date: 2018-12-21 21:31:48
tags: vim
---

工作需要，使用自定义的开发语言, Linux环境。

* notepad++ vs vim
    * vim自带，缺少语法高亮、自动补全。
    * notepad++方便，适合vim不熟悉的人，但上传到linux可能存在编码问题  
其实只要设置好，都可以，哪款编辑器方便就行。
以下内容只涉及**语法高亮**和**自动补全**,**snippets**后期补上

## 语法高亮  
### notepad++  
![notepad_highlight.png](/images/notepad_highlight.png "语法高亮")

### vim  
如果自定义语言完全不同于现有任何格式（如xml json yaml ini等），那比较麻烦，可能需要自己写。我们讨论格式类似的情况：

1. 语法文件  
`/usr/share/vim/{vim版本}/syntax`目录下存在以支持的语法文件，如果你所使用的文件格式与其中某种雷同的话，可以复制一份，自己添加你的关键词。 
如 ` cat /usr/share/vim/vim80/syntax/conf.vim`    

    ```plain
    " Vim syntax file
    " Language:     generic configure file
    " Maintainer:   Bram Moolenaar <Bram@vim.org>
    " Last Change:  2005 Jun 20

    " Quit when a (custom) syntax file was already loaded
    if exists("b:current_syntax")
    finish
    endif

    syn keyword     confTodo        contained TODO FIXME XXX
    " Avoid matching "text#text", used in /etc/disktab and /etc/gettytab
    syn match       confComment     "^#.*" contains=confTodo
    syn match       confComment     "\s#.*"ms=s+1 contains=confTodo
    syn region      confString      start=+"+ skip=+\\\\\|\\"+ end=+"+ oneline
    syn region      confString      start=+'+ skip=+\\\\\|\\'+ end=+'+ oneline

    " Define the default highlighting.
    " Only used when an item doesn't have highlighting yet
    hi def link confComment Comment
    hi def link confTodo    Todo
    hi def link confString  String

    let b:current_syntax = "conf"

    " vim: ts=8 sw=2
    ```

* `syn match` 匹配关键词  
* `hi def link` 高亮  

2. 高亮设置  
  用户`vimrc`文件中添加如下设置
    ```plain
    "开启语法高亮
    syntax on 
    " Your Languare
    " file_ext 替换为你的文件后缀
    " file_type 替换为你刚设置的语法类型
    au BufNewFile,BufRead *.{file_ext}  setf {filetype}
    ```

## 自动补全  
### notepad++  
该`{notepad++安装目录}\plugins\APIs`目录下保存的自动补全的关键字，使用`xml`格式。

基本格式：
```xml
	<AutoComplete>
		<Environment ignoreCase="no" startFunc="(" stopFunc=")" paramSeparator="," additionalWordChar = "." />
		<KeyWord name="ArithmeticError" func="yes">
			<Overload retVal="" descr="Base class for arithmetic errors.">
			</Overload>
		</KeyWord>
    </AutoComplete>
    <!-- keyword 关键字 func 如果未yes，那么Overload中描述的是函数返回值和说明，目前好像由于tinyxml的原因，不支持中文 -->
```

创建你自定义语言对应的xml文件（本人未尝试使用不同命名，有兴趣的可以试试），将高亮的关键字按上面格式加入文件中。

**开启notepad++ 自动补全功能**

### vim

* omni
`.vimrc`文件中加入 `setlocal omnifunc=syntaxcomplete#Complete`

*此方法我未成功，在此仅记录*

* user defined
该方法源自[vi stackexchange](https://vi.stackexchange.com/questions/4584/how-to-create-my-own-autocomplete-function )

1. 建立自己的关键字文件 keyword.txt
2. 在用户.vim 目录下创建补全文件`after/ftplugin/language.vim`,内容如下  
    ```plain
    setlocal complete+=k
    setlocal dictionary+=/path/to/keyword.txt
    setlocal iskeyword+=-
    ```
    
3. 插入模式`<C-n>`或`<C-p>`补全

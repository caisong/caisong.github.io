title: deploy django website to apache2
date: 2018-04-01 20:55:39
tags: django, apach2
---

最近学习使用django，尝试部署至apaache2。

1. required env
```bash
sudo apt-get install apache2 libapache2-mod-wsgi-py3
sudo pip install django
```

2. 拷贝目录至`/var/www`(apache2的默认网站目录)，之前未拷贝，在其他目录尝试了几次都未成功。

3. 添加配置website
	1. 配置网站配置
		```bash
		cd /etc/apache2/sites-avaliable/ && cp 000-default.conf yoursite.conf
		```

	2. 配置详情
	```xml
	<VirtualHost *:80>
		ServerName 127.0.0.1
		ServerAdmin webmaster@localhost
		# 网站根目录, 请根据自身目录配置，下同
		DocumentRoot /var/www/web
		
		#静态文件目录
		Alias /static/ /var/www/web/static/
		
		<Directory /var/www/web/static/>
			Require all granted
		</Directory>
		
		WSGIScriptAlias / /var/www/web/web/wsgi.py

		<Directory /var/www/web/web/>
		<Files wsgi.py>
			Require all granted
		</Files>
		</Directory>
	</VirtualHost>
	``` 

	3. 查看django wigi.py文件
	  默认生成的文件没有问题，不需要修改即可运行。
	  ```python
	  import os
      from os.path import join, dirname,abspath
      
      from django.core.wsgi import get_wsgi_application
      
      
      PROJECT_DIR = dirname(dirname(abspath(__file__)))
      import sys
      sys.path.insert(0, PROJECT_DIR)
      
      os.environ.setdefault("DJANGO_SETTINGS_MODULE", "web.settings")
      
      application = get_wsgi_application()

	  ```

	4. 配置django环境
	进入你的网站根目录
	```bash
	#static files
	#前提在你的setting.py文件中设置好STATIC_ROOT目录
	python manage.py collectstatic

	python manage.py migrate
	```

	5. 修改目录权限为`www-data`
	```bash
	sudo chown www-data:www-data webroot
	# 如果存在数据库，请同时修改相应权限
	```

	6. 使网站生效
	```bash
		sudo a2ensite yoursite.conf
		service apache2 reload
	```


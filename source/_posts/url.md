---
title: django URL 匹配及参数传递
date: 2018-10-15 21:35:39
tags: django
---

## URL 匹配

```python
from django.urls import path, re_path, include 
from . import views

urlpatterns = [
    path('articles/<int:year>/<int:month>/<slug:slug>/', views.article_detail),

    re_path(r'^articles/(?P<year>[0-9]{4})/(?P<month>[0-9]{2})/(?P<slug>[\w-]+)/$', views.article_detail),

    path('contact/', include('contact.urls')),

    path('blog/<int:year>/', views.year_archive, {'foo': 'bar'}),
]
```

## 参数传递

1. URL匹配中带参数的，直接在参数中  
2. URL中仅匹配头部，其他参数`?`之后的，无论客户端发起何种HTTP请求，参数在URL中，都在request.GET.kwargs中获取  
3. 其他在参数在请求体中的，request.POST或request.body中获取,两者差别  
    ```plain
    HttpRequest.body
    The raw HTTP request body as a byte string. This is useful for processing data in different ways than conventional HTML forms: binary images, XML payload etc. For processing conventional form data, use HttpRequest.POST.
    ```



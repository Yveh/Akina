"""Akina URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, re_path

from . import view
from . import captcha

urlpatterns = [
    path(r'', view.startRender),
    path(r'login/', view.loginRender),
    path(r'train/', view.trainRender),
    path(r'ticket/', view.ticketRender),
    path(r'manage/', view.manageRender),
    path(r'person/', view.personRender),
    path(r'logout/', view.logoutRender),
    path(r'clean/', view.cleanRender),
    path(r'base/', view.baseRender),
    path(r'addtrain/', view.addtrainRender),
    path(r'aboutus/', view.aboutusRender),
    re_path(r'^', view.page_not_foundRender),
]

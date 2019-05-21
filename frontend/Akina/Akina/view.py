from django.http import HttpResponse 
from django.shortcuts import render 

def loginRender(request): 
    return render(request, 'login.html')

def indexRender(request):
    return render(request, 'index.html')

def baseRender(request):
    return render(request, 'base.html')
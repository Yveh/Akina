from django.http import HttpResponse 
from django.shortcuts import render 

def login(request): 
    return render(request, 'login.html')

def index(request):
    if request.method == 'POST':
        for i in request.POST:
            print(i, request.POST.get(i))
    return render(request, 'index.html')

from django.http import HttpResponse 
from django.shortcuts import render
from . import uclient
from . import inputchecker

def loginRender(request):
    if request.method == 'POST':
        uname = request.POST.get('name')
        upassword = request.POST.get('password')
        uemail = request.POST.get('email')
        uphone = request.POST.get('phone')
        print('register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
    return render(request, 'login.html')

def indexRender(request):
    if (request.method == 'POST'):
        uid = request.POST.get('id')
        upassword = request.POST.get('password')
        print('command:' + 'login' + ' ' + uid + ' ' + upassword + '\n')
        c = {}
        if (not inputchecker.idChecker(uid)):
            c["message"] = 'invalid id'
            return render(request, 'login.html', c)
        if (not inputchecker.passwordChecker(upassword)):
            c["message"] = 'invalid password'
            return render(request, 'login.html', c)
        ret = uclient.post_and_get('login' + ' ' + uid + ' ' + upassword + '\n')
        if (ret != '1'):
            c["message"] = 'user not exist or wrong password'
            return render(request, 'login.html', c)
    return render(request, 'index.html')

def baseRender(request):
    return render(request, 'base.html')
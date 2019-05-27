from django.http import HttpResponse 
from django.shortcuts import render
from . import uclient
from . import inputchecker

def loginRender(request):
    if request.session.get('logged_in', None):
        return redirect('/index')

    if request.method == 'POST':
        uname = request.POST.get('username')
        upassword = request.POST.get('password')
        uemail = request.POST.get('email')
        uphone = request.POST.get('phone')
        c = {}
        if (not inputchecker.nameChecker(uname)):
            c['message'] = 'username'
            return render(request, 'login.html', c)
        if (not inputchecker.passwordChecker(upassword)):
            c['message'] = 'password'
            return render(request, 'login.html', c)
        if (not inputchecker.emailChecker(uemail)):
            c['message'] = 'email'
            return render(request, 'login.html', c)
        if (not inputchecker.phoneChecker(uphone)):
            c['message'] = 'phone'
            return render(request, 'login.html', c)

        print('command' + ' ' + 'register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
        ret = uclient.post_and_get('command' + ' ' + 'register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
        if (ret != '1'):
            c['message'] = 'rejected'
            return render(request, 'login.html', c)

        c['message'] = 'success'
        return render(request, 'login.html', c)

    return render(request, 'login.html')

def indexRender(request):
    if request.session.get('logged_in', None):
        return render(request, 'index.html')

    if (request.method == 'POST'):
        uid = request.POST.get('id')
        upassword = request.POST.get('password')
        c = {}
        if (not inputchecker.idChecker(uid)):
            c['message'] = 'id'
            return render(request, 'login.html', c)
        if (not inputchecker.passwordChecker(upassword)):
            c['message'] = 'password'
            return render(request, 'login.html', c)
        
        print('command:' + 'login' + ' ' + uid + ' ' + upassword + '\n')
        ret = uclient.post_and_get('login' + ' ' + uid + ' ' + upassword + '\n')
        if (ret != '1'):
            c['message'] = 'rejected'
            return render(request, 'login.html', c)
        
        print('cmmand:' + 'query_profile' + uid + '\n')
        ret = uclient.post_and_get('query_profile' + uid + '\n')

        request.session['logged_in'] = True
        request.session['privilege'] = int(ret)
        
    return render(request, 'index.html')

def baseRender(request):
    return render(request, 'base.html')

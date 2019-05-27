from django.http import HttpResponse 
from django.shortcuts import render
from django.shortcuts import redirect

from . import uclient
from . import inputchecker

def loginRender(request):
    if request.session.get('logged_in', None):
        return redirect('index/')

    if request.method == 'POST':
        uname = request.POST.get('username')
        upassword = request.POST.get('password')
        uemail = request.POST.get('email')
        uphone = request.POST.get('phone')
        c = {}
        w = []
        if (not inputchecker.nameChecker(uname)):
            w.append('inputUsernameR')
        if (not inputchecker.passwordChecker(upassword)):
            w.append('inputPasswordR')
        if (not inputchecker.emailChecker(uemail)):
            w.append('inputEmailR')
        if (not inputchecker.phoneChecker(uphone)):
            w.append('inputPhoneR')
        c['message'] = w
        if (w):
            render(request, 'login.html', c)

        print('command' + ' ' + 'register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
        ret = uclient.post_and_get('command' + ' ' + 'register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
        if (ret != '1'):
            w.append('inputUsernameR')
            w.append('inputPasswordR')
            w.append('inputEmailR')
            w.append('inputPhoneR')
            c['message'] = w
        return render(request, 'login.html', c)

    return render(request, 'login.html')

def indexRender(request):
    if request.session.get('logged_in', None):
        return render(request, 'index.html')

    if (request.method == 'POST'):
        uid = request.POST.get('id')
        upassword = request.POST.get('password')
        c = {}
        w = []
        if (not inputchecker.idChecker(uid)):
            w.append('inputId')
        if (not inputchecker.passwordChecker(upassword)):
            w.append('inputPassword')
        c['message'] = w
        if (w):
            render(request, 'login.html', c)

        print('command:' + 'login' + ' ' + uid + ' ' + upassword + '\n')
        ret = uclient.post_and_get('login' + ' ' + uid + ' ' + upassword + '\n')
        if (ret != '1'):
            w.append('inputId')
            w.append('inputPassword')
            c['message'] = w
            return render(request, 'login.html', c)
        else:
            print('cmmand:' + 'query_profile' + ' ' + uid + '\n')
            ret = uclient.post_and_get('query_profile' + uid + '\n')
            request.session['logged_in'] = True
            request.session['privilege'] = ret[-1]
            return render(request, 'index.html')
        
    return render(request, 'index.html')

def baseRender(request):
    return render(request, 'base.html')

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
            return render(request, 'login.html', c)

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
            return render(request, 'login.html', c)

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

def trainRender(request):
    if (request.method == 'POST'):
        uloc1 = request.POST.get('loc1')
        uloc2 = request.POST.get('loc2')
        utransfer = request.POST.get('transfer')
        udate = request.POST.get('date')
        ucatalog = request.POST.get('catalog')
        c = {}
        w = []
        if (uloc1 == uloc2):
            w.append('loc1')
            w.append('loc2')
        else:
            if (not inputchecker.locChecker(uloc1)):
                w.append('loc1')
            if (not inputchecker.locChecker(uloc2)):
                w.append('loc2')
        if (utransfer != 'on' and utransfer != 'off' or utransfer == 'on' and not request.session.get('logged_in')):
            w.append('transfer')
        if (not inputchecker.dateChecker(udate)):
            w.append('date')
        uucatalog = ''
        for i in ucatalog:
            uucatalog = uucatalog + i
        if (not inputchecker.catalogChecker(uucatalog)):
            w.append('catalog')
        c['message'] = w
        if (w):
            return render(request, 'index.html', c)

        if (utransfer == 'on'):
            print('command:' + 'query_transfer' + ' ' + loc1 + ' ' + loc2 + ' ' + date + ' ' + uucatalog + '\n')
            ret = uclient.post_and_get('query_transfer' + ' ' + loc1 + ' ' + loc2 + ' ' + date + ' ' + uucatalog + '\n')
            print(ret)
            return render(request, 'train.html')
        else:
            print('command:' + 'query_ticket' + ' ' + loc1 + ' ' + loc2 + ' ' + date + ' ' + uucatalog + '\n')
            ret = uclient.post_and_get('query_ticket' + ' ' + loc1 + ' ' + loc2 + ' ' + date + ' ' + uucatalog + '\n')
            print(ret)
            return render(request, 'train.html')

    return render(request, 'index.html')

def baseRender(request):
    return render(request, 'base.html')

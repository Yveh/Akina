from django.http import HttpResponse 
from django.shortcuts import render
from django.shortcuts import redirect
import json

from . import uclient
from . import inputchecker

def startRender(request):
    return render(request, 'start.html')

def loginRender(request):
    if (request.session.get('uid')):
        return redirect('/train/')

    c = {}
    w = []

    if (request.method == 'POST'):
        if (request.POST.get('type') == '注册'):
            uname = request.POST.get('username')
            upassword = request.POST.get('password')
            uemail = request.POST.get('email')
            uphone = request.POST.get('phone')
            
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
            if (ret != '1\n'):
                w.append('inputUsernameR')
                w.append('inputPasswordR')
                w.append('inputEmailR')
                w.append('inputPhoneR')
                c['message'] = w
            return render(request, 'login.html', c)
        elif (request.POST.get('type') == '登录'):
            uid = request.POST.get('id')
            upassword = request.POST.get('password')

            if (not inputchecker.idChecker(uid)):
                w.append('inputId')
            if (not inputchecker.passwordChecker(upassword)):
                w.append('inputPassword')
            c['message'] = w
            if (w):
                return render(request, 'login.html', c)
            print('command:' + 'login' + ' ' + uid + ' ' + upassword + '\n')
            ret = uclient.post_and_get('login' + ' ' + uid + ' ' + upassword + '\n')
            if (ret != '1\n'):
                w.append('inputId')
                w.append('inputPassword')
                c['message'] = w
                return render(request, 'login.html', c)
            else:
                print('cmmand:' + 'query_profile' + ' ' + uid + '\n')
                ret = uclient.post_and_get('query_profile' + ' ' + uid + '\n').split()
                print(ret)
                request.session['uid'] = uid
                request.session['uname'] = ret[0]
                request.session['uemail'] = ret[1]
                request.session['uphone'] = ret[2]
                request.session['uprivilege'] = ret[3]
                return redirect('/train/')
    
    return render(request, 'login.html', c)

def logoutRender(request):
    request.session.flush()
    print(request.META.get('HTTP_REFERER'))
    return redirect(request.META.get('HTTP_REFERER'))

def trainRender(request):
    c = {}
    w = []
    c['uid'] = request.session.get('uid')
    c['uname'] = request.session.get('uname')
    c['uprivilege'] = request.session.get('uprivilege')

    if (request.method == 'POST'):
        uloc1 = request.POST.get('loc1')
        uloc2 = request.POST.get('loc2')
        utransfer = request.POST.get('transfer')
        udate = request.POST.get('date')
        ucatalog = request.POST.getlist('catalog')

        if (uloc1 == uloc2):
            w.append('loc1')
            w.append('loc2')
        else:
            if (not inputchecker.locChecker(uloc1)):
                w.append('loc1')
            if (not inputchecker.locChecker(uloc2)):
                w.append('loc2')
        if (utransfer == 'on' and not request.session.get('uid')):
            w.append('loginbutton')
            w.append('registerbutton')
        if (not inputchecker.dateChecker(udate)):
            w.append('date')
        uucatalog = ''
        print(ucatalog)
        for i in ucatalog:
            uucatalog = uucatalog + i
        if (not inputchecker.catalogChecker(uucatalog)):
            w.append('catalog')
        c['message'] = w
        if (w):
            return render(request, 'train.html', c)

        #TO DO: show the results / buy
        c['querydone'] = True
        print(utransfer)
        if (utransfer == 'on'):
            print('command:' + 'query_transfer' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n')
            ret = uclient.post_and_get('query_transfer' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
            """
            for i in ret:
                utrain = i.split()
                for j in utrain:
                    print(j)
            """
            rrr = ret[0].split()
            print(rrr[1])
            return render(request, 'train.html', c)
        else:
            print('command:' + 'query_ticket' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n')
            ret = uclient.post_and_get('query_ticket' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
            table = []
            for i in ret:
                utrain = i.split()
                if not utrain:
                    break
                tmp = {}
                tmp['trainID'] = utrain[0]
                tmp['loc1'] = utrain[1]
                tmp['date1'] = utrain[2]
                tmp['time1'] = utrain[3]
                tmp['loc2'] = utrain[4]
                tmp['date2'] = utrain[5]
                tmp['time2'] = utrain[6]
                tmp['operator'] = 'qwq'
                table.append(tmp)
            c['table'] = json.dumps(table)
            return render(request, 'train.html', c)

    return render(request, 'train.html', c)

def ticketRender(request):
    c = {}
    w = []
    c['uid'] = request.session.get('uid')
    
    if (request.method == 'POST'):
        uid = request.POSt.get('id')
        udate = request.POSt.get('date')
        ucatalog = request.POST.get('catalog')

        if (not inputchecker.idChecker(uid)):
            w.append('id')
        if (not inputchecker.dateChecker(udate)):
            w.append('date')
        uucatalog = ''
        for i in ucatalog:
            uucatalog = uucatalog + i
        if (not inputchecker.catalogChecker(uucatalog)):
            w.append('catalog')
        c['message'] = w
        if (w):
            return render(request, 'ticket.html', c)
        
        #TO DO: show the results / refund
        print('command:' + 'query_order' + ' ' + uid + ' ' + udate + ' ' + uucatalog + '\n')
        ret = uclient.post_and_get('query_order' + ' ' + uid + ' ' + udate + ' ' + uucatalog + '\n')
        print(ret)
        return render(request, 'ticket.html', c)

    return render(request, 'ticket.html', c)

def manageRender(request):
    """
    if (request.session.get('privilege') != '2'):
        return redirect('/')
    """
    print("ok")
    c = {}
    w = []
    c['uid'] = request.session.get('uid')

    if (request.method == 'POST'):
        uid = request.POST.get('id')
        if (not inputchecker.idChecker):
            w.append('id')
        c['message'] = w
        if (w):
            return render(request, 'ticket.html', c)
        
        #TO DO: sale/delete/modify
        print('command:' + 'query_train' + ' ' + uid + '\n')
        ret = uclient.post_and_get('query_train' + ' ' + uid + '\n')
        print(ret)
        return render(request, 'manage.html')
    return render(request, 'manage.html')

def personRender(request):
    pass

def baseRender(request):
    return render(request, 'base.html')

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
            print('command:' + 'register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
            ret = uclient.post_and_get('register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
            print(ret)
            if (ret == '-1\n'):
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
            print(ret)
            if (ret != '1\n'):
                w.append('inputId')
                w.append('inputPassword')
                c['message'] = w
                return render(request, 'login.html', c)
            else:
                print('command:' + 'query_profile' + ' ' + uid + '\n')
                ret = uclient.post_and_get('query_profile' + ' ' + uid + '\n').split()
                print(ret)
                request.session['uid'] = uid
                request.session['uname'] = ret[0]
                request.session['uemail'] = ret[1]
                request.session['uphone'] = ret[2]
                request.session['uprivilege'] = ret[3]
                request.session['upassword'] = upassword
                return redirect('/train/')
    
    print(request.GET)
    c['flag'] = request.GET.get('flag')
    return render(request, 'login.html', c)

def logoutRender(request):
    request.session.flush()
    print(request.META.get('HTTP_REFERER'))
    return redirect(request.META.get('HTTP_REFERER'))

def trainRender(request):
    c = {}
    w = []
    c['uid'] = request.session.get('uid')
    c['upassword'] = request.session.get('upassword')
    c['uname'] = request.session.get('uname')
    c['uphone'] = request.session.get('uphone')
    c['uemail'] = request.session.get('uemail')
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
        for i in ucatalog:
            uucatalog = uucatalog + i
        if (not inputchecker.catalogChecker(uucatalog)):
            w.append('catalog')
        c['message'] = w
        if (w):
            return render(request, 'train.html', c)

        #TO DO: show the results / buy
        c['querydone'] = True
        if (utransfer == 'on'):
            print('command:' + 'query_transfer' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n')
            ret = uclient.post_and_get('query_transfer' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
            print(ret)
            table = []
            for i in ret:
                utrain = i.split()
                if (len(utrain) <= 1):
                    continue
                tmp = {}
                tmp['trainID'] = utrain[0]
                tmp['loc1'] = utrain[1]
                tmp['date1'] = utrain[2]
                tmp['time1'] = utrain[3]
                tmp['loc2'] = utrain[4]
                tmp['date2'] = utrain[5]
                tmp['time2'] = utrain[6]
                tmp['tleft'] = ''
                tmp['operation'] = r'<button class="btn-primary btn-md">购买</button>'
                for i in range(0, (len(utrain) - 6) // 3):
                    tmp['tleft'] += r'<div><b>' + utrain[7 + i * 3] + r'</b>' + '   ¥' + utrain[7 + i * 3 + 2] + '   ' + utrain[7 + i * 3 + 1] + '张' + r'</div>'
                table.append(tmp)
            c['table'] = json.dumps(table)
            return render(request, 'train.html', c)
        else:
            print('command:' + 'query_ticket' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n')
            ret = uclient.post_and_get('query_ticket' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
            print(ret)
            table = []
            for i in ret:
                utrain = i.split()
                if (len(utrain) <= 1):
                    continue
                tmp = {}
                tmp['trainID'] = utrain[0]
                tmp['loc1'] = utrain[1]
                tmp['date1'] = utrain[2]
                tmp['time1'] = utrain[3]
                tmp['loc2'] = utrain[4]
                tmp['date2'] = utrain[5]
                tmp['time2'] = utrain[6]
                tmp['tleft'] = ''
                tmp['operation'] = r'<button class="btn-primary btn-md">购买</button>'
                for i in range(0, (len(utrain) - 6) // 3):
                    tmp['tleft'] += r'<div><b>' + utrain[7 + i * 3] + r'</b>' + '   ¥' + utrain[7 + i * 3 + 2] + '   ' + utrain[7 + i * 3 + 1] + '张' + r'</div>'
                table.append(tmp)
            c['table'] = json.dumps(table)
            return render(request, 'train.html', c)

    return render(request, 'train.html', c)

def ticketRender(request):
    if (not request.session.get('uid')):
        return redirect('/login/')

    c = {}
    w = []
    c['uid'] = request.session.get('uid')
    c['upassword'] = request.session.get('upassword')
    c['uname'] = request.session.get('uname')
    c['uphone'] = request.session.get('uphone')
    c['uemail'] = request.session.get('uemail')
    c['uprivilege'] = request.session.get('uprivilege')
    
    if (request.method == 'POST'):
        uid = request.session.get('uid')
        udate = request.POST.get('date')
        ucatalog = request.POST.getlist('catalog')

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
        
        c['querydone'] = True
        print('command:' + 'query_order' + ' ' + uid + ' ' + udate + ' ' + uucatalog + '\n')
        ret = uclient.post_and_get('query_order' + ' ' + uid + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
        print(ret)
        table = []
        for i in ret:
            utrain = i.split()
            print(utrain)
            if (len(utrain) <= 1):
                continue
            tmp = {}
            tmp['trainID'] = utrain[0]
            tmp['loc1'] = utrain[1]
            tmp['date1'] = utrain[2]
            tmp['time1'] = utrain[3]
            tmp['loc2'] = utrain[4]
            tmp['date2'] = utrain[5]
            tmp['time2'] = utrain[6]
            tmp['tleft'] = ''
            tmp['operation'] = r'<button type="button" class="btn-primary btn-md" data-toggle="modal" data-target="#myModal">退票</button>'
            for i in range(0, (len(utrain) - 6) // 3):
                tmp['tleft'] += r'<div><b>' + utrain[7 + i * 3] + r'</b>' + '   ¥' + utrain[7 + i * 3 + 2] + '   ' + utrain[7 + i * 3 + 1] + '张' + r'</div>'
            table.append(tmp)
        c['table'] = json.dumps(table)
        return render(request, 'ticket.html', c)

    return render(request, 'ticket.html', c)

def manageRender(request):
    if (not request.session.get('uid')):
        return redirect('/login/')

    c = {}
    w = []
    c['uid'] = request.session.get('uid')
    c['upassword'] = request.session.get('upassword')
    c['uname'] = request.session.get('uname')
    c['uphone'] = request.session.get('uphone')
    c['uemail'] = request.session.get('uemail')
    c['uprivilege'] = request.session.get('uprivilege')

    if (request.method == 'POST'):
        uid = request.POST.get('id')
        uop = request.POST.get('op')
        if (not inputchecker.trainIdChecker(uid)):
            w.append('id')
        c['message'] = w
        if (w):
            return render(request, 'manage.html', c)
        
        c['querydone'] = True
        if (uop == '发售车次'):
            print('command:' + 'sale_train' + ' ' + uid + '\n')
            ret = uclient.post_and_get('sale_train' + ' ' + uid + '\n')
            print(ret)
            return render(request, 'manage.html', c)
        elif (uop == '搜索车次'):
            print('command:' + 'query_train' + ' ' + uid + '\n')
            ret = uclient.post_and_get('query_train' + ' ' + uid + '\n').split('\n')
            print(ret)

            tmp = ret[0].split()
            c['trainID'] = tmp[0]
            c['name'] = tmp[1]
            c['catalog'] = tmp[2]
            c['type'] = []
            for i in range(5, len(tmp)):
                c['type'].append(tmp[i])

            table = []
            for i in range(1, len(ret)):
                tmp = ret[i].split()
                if (not len(tmp)):
                    continue
                qwq = {}
                qwq['loc1'] = tmp[0]
                qwq['time1'] = tmp[1]
                qwq['time2'] = tmp[2]
                qwq['time3'] = tmp[3]
                qwq['price'] = ''
                for j in range(4, len(tmp)):
                    qwq['price'] += r'<div><b>' + tmp[j] + r'</b></div>'
                table.append(qwq)
            c['table'] = json.dumps(table)
            print(c['table'])
            return render(request, 'manage.html', c)

        return render(request, 'manage.html', c)

    return render(request, 'manage.html', c)

def personRender(request):
    if (not request.session.get('uid')):
        return redirect('/login/')
    
    c = {}
    w = []

    c['uid'] = request.session.get('uid')
    c['upassword'] = request.session.get('upassword')
    c['uname'] = request.session.get('uname')
    c['uphone'] = request.session.get('uphone')
    c['uemail'] = request.session.get('uemail')
    c['uprivilege'] = request.session.get('uprivilege')

    if (request.method == 'POST'):
        uid = request.session.get('uid')
        uname = request.POST.get('username')
        if (uname == ''):
            uname = request.session.get('uname')
        uopassword = request.POST.get('opassword')
        unpassword = request.POST.get('npassword')
        print(unpassword)
        if (unpassword == ''):
            unpassword = request.session.get('upassword')
        uemail = request.POST.get('email')
        if (uemail == ''):
            uemail = request.session.get('uemail')
        uphone = request.POST.get('phone')
        if (uphone == ''):
            uphone = request.session.get('uphone')

        if (not inputchecker.nameChecker(uname)):
            w.append('inputUsername')
        if (not inputchecker.passwordChecker(uopassword)):
            w.append('inputoPassword')
        if (not inputchecker.passwordChecker(unpassword)):
            w.append('inputnPassword')
        if (not inputchecker.emailChecker(uemail)):
            w.append('inputEmail')
        if (not inputchecker.phoneChecker(uphone)):
            w.append('inputPhone')
        c['message'] = w
        if (w):
            return render(request, 'person.html', c)
        print('command:' + 'login' + ' ' + uid + ' ' + uopassword + '\n')
        ret = uclient.post_and_get('login' + ' ' + uid + ' ' + uopassword + '\n')
        if (ret != '1\n'):
            w.append('inputoPassword')
        c['message'] = w
        if (w):
            return render(request, 'pperson.html', c)
        print('command:' + 'modify_profile' + ' ' + uid + ' ' + uname + ' ' + unpassword + ' ' + uemail + ' ' + uphone + '\n')
        ret = uclient.post_and_get('modify_profile' + ' ' + uid + ' ' + uname + ' ' + unpassword + ' ' + uemail + ' ' + uphone + '\n')
        request.session['uname'] = uname
        request.session['uemail'] = uemail
        request.session['uphone'] = uphone
        c['uname'] = request.session.get('uname')
        c['uphone'] = request.session.get('uphone')
        c['uemail'] = request.session.get('uemail')
        return render(request, 'person.html', c)

    return render(request, 'person.html', c)

def cleanRender(request):
    request.session.flush()
    print('command:' + 'clean' + '\n')
    ret = uclient.post_and_get('command:' + 'clean' + '\n')
    print(ret)
    return redirect('/')

def baseRender(request):
    return render(request, 'base.html')

def addtrainRender(request):
    if (not request.session.get('uid')):
        return redirect('/login/')
    
    c = {}
    w = []

    c['uid'] = request.session.get('uid')
    c['upassword'] = request.session.get('upassword')
    c['uname'] = request.session.get('uname')
    c['uphone'] = request.session.get('uphone')
    c['uemail'] = request.session.get('uemail')
    c['uprivilege'] = request.session.get('uprivilege')

    if (request.method == "POST"):
        print(request.POST)
        utrainid = request.POST.get('trainId')
        uname = request.POST.get('name')
        ucatalog = request.POST.getlist('catalog')
        uucatalog = ''
        for i in ucatalog:
            uucatalog = uucatalog + i
        utype = request.POST.getlist('type[]')
        uloc1 = request.POST.getlist('loc1[]')
        utime1 = request.POST.getlist('time1[]')
        utime2 = request.POST.getlist('time2[]')
        utime3 = request.POST.getlist('time3[]')
        uprice = request.POST.getlist('price[]')

        # TO DO checker
        command = 'add_train' + ' ' + utrainid + ' ' + uname + ' ' + uucatalog + ' ' + str(len(uloc1)) + ' ' + str(len(utype))
        for i in utype:
            command += ' ' + i
        command += '\n'
        for i in range(0, len(uloc1)):
            command += uloc1[i] + ' '
            if (i == 0):
                command += 'xx:xx '
            else:
                command += utime1[i] + ' '
            if (i == len(uloc1) - 1):
                command += 'xx:xx '
            else:
                command += utime2[i] + ' '
            command += utime3[i] + ' ¥' + uprice[i] + '\n'
        print('command:' + command)
        ret = uclient.post_and_get(command)
        print(ret)
        return redirect('/manage/')

    return render(request, 'addtrain.html', c)

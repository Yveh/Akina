from django.http import HttpResponse 
from django.shortcuts import render
from django.shortcuts import redirect
import hashlib
import json

from . import uclient
from . import inputchecker
from . import captcha

def page_not_foundRender(request):
    return render(request, '404.html')

def startRender(request):
    return render(request, 'start.html')

def loginRender(request):
    if (request.session.get('uid')):
        return redirect('/train/')

    c = {}
    w = []

    ucaptcha = request.POST.get('captcha')
    pass_captcha = True
    if ((not request.session.get('captcha_id')) or (not inputchecker.captchaChecker(ucaptcha)) or (ucaptcha != 'baby' and float(ucaptcha) != captcha.get_captcha_ans(request.session.get('captcha_id')))):
        pass_captcha = False

    cap = captcha.generate_captcha()
    request.session['captcha_id'] = cap[0]
    c['captcha_html'] = cap[1]
   
    if (request.method == 'POST'):
        if (request.POST.get('type') == '注册'):
            uname = request.POST.get('username')
            upassword = hashlib.sha256(request.POST.get('password').encode('utf-8')).hexdigest()[0:19]
            print(upassword)
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
            if (not pass_captcha):
                w.append('inputCaptchaR')

            c['message'] = w
   
            if (w):
                c['show_flag'] = True
                c['show_color'] = '#FF2D2D'
                c['show_message'] = '🐓？'
                c['flag'] = True
                return render(request, 'login.html', c)
            print('command:' + 'register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
            ret = uclient.post_and_get('register' + ' ' + uname + ' ' + upassword + ' ' + uemail + ' ' + uphone + '\n')
            print(ret)
            if (type(ret) != str or ret == '-1\n'):
                '''
                w.append('inputUsernameR')
                w.append('inputPasswordR')
                w.append('inputEmailR')
                w.append('inputPhoneR')
                '''
                c['show_flag'] = True
                c['show_color'] = '#FF2D2D'
                c['show_message'] = '服务器在上物理课🐴'
                c['message'] = w
                c['flag'] = True
            else:
                c['show_flag'] = True
                c['show_color'] = '#66BB44'
                c['show_message'] = "Success! ID: " + str(int(ret))
            return render(request, 'login.html', c)
        elif (request.POST.get('type') == '登录'):
            uid = request.POST.get('id')
            upassword = hashlib.sha256(request.POST.get('password').encode('utf-8')).hexdigest()[0:19]
            print(upassword)

            if (not inputchecker.idChecker(uid)):
                w.append('inputId')
            if (not inputchecker.passwordChecker(upassword)):
                w.append('inputPassword')
            c['message'] = w
            if (w):
                c['flag'] = False
                return render(request, 'login.html', c)
            print('command:' + 'login' + ' ' + uid + ' ' + upassword + '\n')
            ret = uclient.post_and_get('login' + ' ' + uid + ' ' + upassword + '\n')
            print(ret)
            if (ret != '1\n'):
                c['show_flag'] = True
                c['show_color'] = '#FF2D2D'
                c['show_message'] = '🐴？'
                w.append('inputId')
                w.append('inputPassword')
                c['message'] = w
                c['flag'] = False
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
    else:
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
        print(request.POST)
        utype = request.POST.get('op')
        uid = request.session.get('uid')
        if (utype == '购买'):
            utrainID = request.POST.get('trainID')
            uloc1 = request.POST.get('loc1')
            uloc2 = request.POST.get('loc2')
            udate = request.POST.get('date')
            utype = request.POST.get('type')
            print('udate=', udate)
            print('command:' + 'buy_ticket' + ' ' + uid + ' ' + '1' + ' ' + utrainID + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + utype + '\n')
            ret = uclient.post_and_get('buy_ticket' + ' ' + uid + ' ' + '1' + ' ' + utrainID + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + utype + '\n')
            print(ret)
            return render(request, 'train.html', c)
        else:
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

            c['querydone'] = True
            if (utransfer == 'on'):
                print('command:' + 'query_transfer' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n')
                ret = uclient.post_and_get('query_transfer' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
            else:
                print('command:' + 'query_ticket' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n')
                ret = uclient.post_and_get('query_ticket' + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + uucatalog + '\n').split('\n')
            #print(ret)
            table = []
            tableO = []
            for i in range(0, len(ret)):
                utrain = ret[i].split()
                print(utrain)
                if (len(utrain) < 2):
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
                if (utransfer == 'on'):
                    tmp['operation'] = r'<button type="button" class="btn-primary btn-md" data-toggle="modal" data-id="' + str(i) + r'" data-target="#myModal">购买</button>'
                else:
                    tmp['operation'] = r'<button type="button" class="btn-primary btn-md" data-toggle="modal" data-id="' + str(i - 1) + r'" data-target="#myModal">购买</button>'
                tmpO = {}
                tmpO['type'] = []
                tmpO['trainID'] = utrain[0]
                tmpO['loc1'] = utrain[1]
                tmpO['date'] = utrain[2]
                tmpO['loc2'] = utrain[4]
                for i in range(0, (len(utrain) - 6) // 3):
                    tmp['tleft'] += r'<div><b>' + utrain[7 + i * 3] + r'</b>' + '   ¥' + utrain[7 + i * 3 + 2] + '   ' + utrain[7 + i * 3 + 1] + '张' + r'</div>'
                    tmpO['type'].append(utrain[7 + i * 3])
                table.append(tmp)
                tableO.append(tmpO)
            c['table'] = json.dumps(table)
            c['tableO'] = json.dumps(tableO)
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
        utype = request.POST.get('op')
        uid = request.session.get('uid')
        if (utype == "退票"):
            utrainID = request.POST.get('trainID')
            uloc1 = request.POST.get('loc1')
            uloc2 = request.POST.get('loc2')
            udate = request.POST.get('date')
            utype = request.POST.get('type')
            print('command:' + 'refund_ticket' + ' ' + uid + ' ' + '1' + ' ' + utrainID + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + utype + '\n')
            ret = uclient.post_and_get('refund_ticket' + ' ' + uid + ' ' + '1' + ' ' + utrainID + ' ' + uloc1 + ' ' + uloc2 + ' ' + udate + ' ' + utype + '\n')
            return render(request, 'ticket.html', c)
        else:
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
            table = []
            tableO = []
            print(len(ret))
            for i in range(1, len(ret)):
                utrain = ret[i].split()
                if (len(utrain) < 2):
                    continue
                print(utrain)
                tmp = {}
                tmp['trainID'] = utrain[0]
                tmp['loc1'] = utrain[1]
                tmp['date1'] = utrain[2]
                tmp['loc2'] = utrain[4]
                tmp['date2'] = utrain[5]
                tmp['time1'] = utrain[3]
                tmp['time2'] = utrain[6]
                tmp['tleft'] = ''
                if (utrain[7 + i * 3] == '胡其图'):
                    tmp['operation'] = r'<button type="button" class="btn-primary btn-md" data-toggle="modal" data-id="' + str(i - 1) + r'" data-target="#myModal">退课</button>'
                else:
                    tmp['operation'] = r'<button type="button" class="btn-primary btn-md" data-toggle="modal" data-id="' + str(i - 1) + r'" data-target="#myModal">退票</button>'
                tmpO = {}
                tmpO['type'] = []
                tmpO['trainID'] = utrain[0]
                tmpO['loc1'] = utrain[1]
                tmpO['date'] = utrain[2]
                tmpO['loc2'] = utrain[4]
                for i in range(0, (len(utrain) - 6) // 3):
                    if (int(utrain[7 + i * 3 + 1]) > 0):
                        tmp['tleft'] += r'<div><b>' + utrain[7 + i * 3] + r'</b>' + '   ¥' + utrain[7 + i * 3 + 2] + '   ' + utrain[7 + i * 3 + 1] + '张' + r'</div>'
                        tmpO['type'].append(utrain[7 + i * 3])
                table.append(tmp)
                tableO.append(tmpO)
            c['table'] = json.dumps(table)
            c['tableO'] = json.dumps(tableO)

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
        
        if (uop == '发售车次'):
            print('command:' + 'sale_train' + ' ' + uid + '\n')
            ret = uclient.post_and_get('sale_train' + ' ' + uid + '\n')
            print(ret)
            if (ret != '1\n'):
                w.append('id')
                c['message'] = w
            return render(request, 'manage.html', c)
        elif (uop == '搜索车次'):
            print('command:' + 'query_train' + ' ' + uid + '\n')
            ret = uclient.post_and_get('query_train' + ' ' + uid + '\n').split('\n')
            print(ret)
            if (len(ret) <= 2):
                w.append('id')
                c['message'] = w
                return render(request, 'manage.html', c)
            c['querydone'] = True
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
                if (len(tmp) < 2):
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
        uopassword = hashlib.sha256(request.POST.get('opassword').encode('utf-8')).hexdigest()[0:19]
        unpassword = hashlib.sha256(request.POST.get('npassword').encode('utf-8')).hexdigest()[0:19]
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
            return render(request, 'person.html', c)
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
    
def aboutusRender(request):
    c = {}
    c['uid'] = request.session.get('uid')
    c['upassword'] = request.session.get('upassword')
    c['uname'] = request.session.get('uname')
    c['uphone'] = request.session.get('uphone')
    c['uemail'] = request.session.get('uemail')
    c['uprivilege'] = request.session.get('uprivilege')
    return render(request, 'aboutus.html', c)

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
            command += 'xx:xx'
            tmp = uprice[i].split()
            for j in tmp:
                if (j):
                    command += ' aaa' + j
            command += '\n'
        print('command:' + command)
        ret = uclient.post_and_get(command)
        print(ret)
        return redirect('/manage/')

    return render(request, 'addtrain.html', c)

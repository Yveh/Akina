import re

def stringChecker(x):
    return type(x) == type('lily')

def captchaChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 40 and (x == 'baby' or re.match("^[+-]?([0-9]*[.]?[0-9]*)$", x) != None)

def idChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 10 and re.match(r"^[1-9][0-9]+$", x) != None

def nameChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 40 and re.match(r"^\S+$", x) != None

def passwordChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 20 and re.match(r"^\S+$", x) != None

def emailChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 20 and re.match("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$", x) != None

def phoneChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 20 and re.match(r"^\S+$", x) != None

def locChecker(x):
    return stringChecker(x) and len(x) > 0 and len(x) <= 20 and re.match(r"^\S+$", x) != None

def dateChecker(x):
    return stringChecker(x) and len(x) == 10 and re.match(r"^((((1[6-9]|[2-9]\d)\d{2})-(0?[13578]|1[02])-(0?[1-9]|[12]\d|3[01]))|(((1[6-9]|[2-9]\d)\d{2})-(0?[13456789]|1[012])-(0?[1-9]|[12]\d|30))|(((1[6-9]|[2-9]\d)\d{2})-0?2-(0?[1-9]|1\d|2[0-8]))|(((1[6-9]|[2-9]\d)(0[48]|[2468][048]|[13579][26])|((16|[2468][048]|[3579][26])00))-0?2-29))$", x) != None

def catalogChecker(x):
    return stringChecker(x) and len(x) < 10 and re.match(r"^(?!.*?([CDGKOTZ]).*?\1.*?$)[CDGKOTZ]+$", x) != None

def trainIdChecker(x):
    return stringChecker(x) and len(x) < 20 and re.match(r"^\S+$", x) != None


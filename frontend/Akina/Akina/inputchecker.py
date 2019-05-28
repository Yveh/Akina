import re

def idChecker(x):
    return len(x) > 0 and len(x) <= 10 and re.match(r"^[1-9][0-9]+$", x) != None and int(x) >= 2019

def nameChecker(x):
    return len(x) > 0 and len(x) <= 40 and re.match(r"^\S+$", x) != None

def passwordChecker(x):
    return len(x) > 0 and len(x) <= 20 and re.match(r"^\S+$", x) != None

def emailChecker(x):
    return len(x) > 0 and len(x) <= 20 and re.match("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$", x) != None

def phoneChecker(x):
    return len(x) > 0 and len(x) <= 20 and re.match(r"^\S+$", x) != None

def locChecker(x):
    return 1

def dateChecker(x):
    return 1

def catalogChecker(x):
    return 0

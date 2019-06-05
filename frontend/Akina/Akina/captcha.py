import random

def generate_captcha():
    x = (random.randint(1, 5) * 2, random.randint(1, 5), random.randint(1, 5))
    return (x, '$\\int_0^{{ {0} }} {1} x + {2} \\, dx$'.format(str(x[0]), str(x[1]), str(x[2])))

def get_captcha_ans(x):
    return float(x[0] * x[2] + 0.5 * x[0] * x[0] * x[1])


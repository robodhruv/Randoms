"""
Python script to generate 1000 random test cases
for computing the GCD of 16-bit numbers.

Author: Dhruv Ilesh Shah
"""
from random import random

f = open('Tracefile.txt','w')
serial_stream = 16 # Length of the stream (> 2)
num_cases = 1000 # Number of test cases

def gcd(a,b):
    while b!=0:
        tmp = b
        b = a%b
        a = tmp
    return a

def pr(a):
    q = gcd(a[0],a[1])
    for i in range(2,serial_stream):
        q = gcd(q,a[i])
    outstr = ''
    for x in a:
        outstr += '{:016b} '.format(x)
    outstr += '{:016b}\n'.format(q)
    f.write(outstr)

def generate(a):
    l = [];
    for i in range(serial_stream):
        q = int(random()*511)+1
        l.append(q*a)
    pr(l)

for i in range(num_cases):
    generate(int(random()*127)+1)

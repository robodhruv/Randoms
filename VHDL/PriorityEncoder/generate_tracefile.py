def tobinary(n, width):
    s = ''
    for i in range(width):
        s = s + str(n % 2)
        n = n / 2
        s = s[:: -1]
    return s
 
f = open('TRACEFILE.txt', 'w')
f. seek(0)
f. truncate()
for i in range(256):
    x = tobinary(i, 8)
    s = '000'
    if x == '00000000':
        N = '1'
    else:
        N = '0'
 
    if x[7] == '1':
        s = '000'
    elif x[6] == '1':
        s = '001'
    elif x[5] == '1':
        s = '010'
    elif x[4] == '1':
        s = '011'
    elif x[3] == '1':
        s = '100'
    elif x[2] == '1':
        s = '101'
    elif x[1] == '1':
        s = '110'
    elif x[0] == '1':
        s = '111'
    f. write(x + " " + s + N +" " + "1111" +  "\n")
f. close()

def crypto():
    fin=open('input.txt')
    fout=open('output.txt','w')
    o=''
    k=fin.readline()[:-1]
    fout.write(k+'\n')
    l=len(k)
    a=' .'
    for i in range(10):
        a=a+str(i)
    for i in range(ord('a'),ord('z')+1):
        a=a+chr(i)
    for i in range(ord('A'),ord('Z')+1):
        a=a+chr(i)
    while True:
        t=fin.readline()
        if not t:
            break
        for i in range(len(t)):
            if t[i].isalpha() or t[i].isdigit() or t[i]==' ' or t[i]=='.':
                k_=k[i%l]
                nk=a.find(k_)
                nb=a.find(t[i])
                o=o+a[nk^nb]
            else:
                o=o+t[i]
            fout.write(o)
            o=''
    fin.close()
    fout.close()

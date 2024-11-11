def crypto():
    fin=open('input.txt')
    fout=open('output.txt','w')
    na=ord('a')
    k=-1
    while True:
        a=fin.readline()
        if not a:
            break
        i=0
        k=k+1
        if k==5:
            k=0
        while a[0]==' ':
            i=i+1
            a=a[4:]
        fout.write(chr(na+i+k)+a)
    fin.close()
    fout.close()

def decrypto():
    fin=open('input.txt')
    fout=open('output.txt','w')
    na=ord('a')
    k=-1
    while True:
        a=fin.readline()
        if not a:
            break
        k=k+1
        if k==5:
            k=0
        i=4*(ord(a[0])-na-k)
        fout.write(' '*i+a[1:])
    fin.close()
    fout.close()

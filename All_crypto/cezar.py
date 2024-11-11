def crypto():
    fin=open('input.txt')
    fout=open('output.txt','w')
    s_,f=map(int,fin.readline().split())
    if f==1:
        fout.write(str(s_)+' ')
        fout.write('2'+'\n')
    s_=list(str(s_))
    s_=[int(x) for x in s_]
    l=len(s_)
    o=''
    if f==1:
        while True:
            a=fin.readline()
            if not a:
                break
            for i in range(len(a)):
                if a[i].isalpha():
                    s=s_[i%l]
                    if 'a'<=a[i]<=chr((ord('z')-s)) or 'A'<=a[i]<=chr((ord('Z')-s)):
                        x=ord(a[i])+s
                    else:
                        x=ord(a[i])+s-26
                    o=o+chr(x)
                elif a[i].isdigit():
                    s=s_[i%l]
                    if '0'<=a[i]<=chr((ord('9')-s)):
                        x=ord(a[i])+s
                    else:
                        x=ord(a[i])+s-10
                    o=o+chr(x)
                else:
                    o=o+a[i]
            fout.write(o)
            o=''
    else:
        while True:
            a=fin.readline()
            if not a:
                break
            for i in range(len(a)):
                if a[i].isalpha():
                    s=s_[i%l]
                    if chr(ord('a')+s)<=a[i]<='z' or chr(ord('A')+s)<=a[i]<='Z':
                        x=ord(a[i])-s
                    else:
                        x=ord(a[i])-s+26
                    o=o+chr(x)
                elif a[i].isdigit():
                    s=s_[i%l]
                    if chr(ord('0')+s)<=a[i]<='9':
                        x=ord(a[i])-s
                    else:
                        x=ord(a[i])-s+10
                    o=o+chr(x)
                else:
                    o=o+a[i]
            fout.write(o)
            o=''
    fin.close()
    fout.close()

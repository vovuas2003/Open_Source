def crypto():
    ru=['ф','и','с','в','у','а','п','р','ш','о','л','д','ь','т','щ','з','й','к','ы','е','г','м','ц','ч','н','я']
    en=[]
    a=''
    fin=open('input.txt')
    fout=open('output.txt','w')
    for i in range(ord('A'),ord('Z')+1):
        en.append(chr(i))
    while True:
        s=fin.readline()
        if not s:
            break
        s=list(s)
        s=[x.upper() for x in s]
        for i in range(len(s)):
            if s[i].isalpha():
                a=a+ru[en.index(s[i])]
            elif s[i]=='[' or s[i]=='{':
                a=a+'х'
            elif s[i]==']' or s[i]=='}':
                a=a+"ъ"
            elif s[i]==';' or s[i]==':':
                a=a+'ж'
            elif s[i]=="'" or s[i]=='"':
                a=a+'э'
            elif s[i]=='`' or s[i]=='~':
                a=a+'ё'
            elif s[i]==',' or s[i]=='<':
                a=a+'б'
            elif s[i]=='.' or s[i]=='>':
                a=a+'ю'
            else:
                a=a+s[i]
        fout.write(a)
        a=''
    fin.close()
    fout.close()

def s4et():
    import time
    import random
    op='+-*'
    yes=0
    tim=0
    no=0
    err=0
    print('Устный счёт на время. На ответ 5 секунд\nДля выхода введите q вместо ответа')
    print('Нажмите ENTER, чтобы начать',end='')
    input()
    while True:
        type_op=op[random.randint(0,2)]
        x1=random.randint(10,99)
        if type_op=='*':
            x2=random.randint(2,9)
        else:
            x2=random.randint(10,99)
        if type_op=='+':
            res=x1+x2
        elif type_op=='-':
            res=x1-x2
        else:
            res=x1*x2
        print(x1,' ',type_op,' ',x2,' = ',sep='',end='')
        t1=time.time()
        s=input()
        t2=time.time()
        t=t2-t1
        if s=='q':
            break
        else:
            try:
                s=int(s)
                if s==res:
                    if t<=5:
                        print('Правильно')
                        yes+=1
                    else:
                        print('Правильно. Время просрочено')
                        tim+=1
                else:
                    print('Неправильно. Должно быть ',res,sep='')
                    no+=1
            except:
                print('Некорректный ввод')
                err=err+1
    print('Правильные:',yes)
    print('Просрочено:',tim)
    print('Неправильные:',no)
    print('Ошибки ввода:',err)

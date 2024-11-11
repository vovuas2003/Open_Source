def hash(n,m,s):
    a=0
    for i in range(len(s)):
    	a=((a*n)%m+ord(s[i]))%m
    return a
def check():
    import getpass
    output=True
    m=2**61-1
    n=257
    print('Программа шифрования\n(C)Наумкин Владимир\n')
    l=getpass.getpass('Введите логин: ')
    l_h=hash(n,m,l)
    if l_h!=123456789: #DELETED HASH OF MY LOGIN => run hash(n,m,"your_login") and edit 123456789
        output=False
    p=getpass.getpass('Введите пароль: ')
    p_h=hash(n,m,p)
    if p_h!=123456789: #DELETED HASH OF MY PASSWORD => run hash(n,m,"your_password") and edit 123456789
        output=False
    if output:
        print('Успешный вход')
    else:
        print('Неверный логин или пароль')
    return output

if check():
    import cezar
    import vernam
    import translite
    import tabs
    import nebag
    vvod_1=['0','1','2','3','4','1337']
    vvod_2=['0','1']
    c_info='Для зашифровки:\nв входной файл в первой строке написать через пробел\nкод (число) и 1 (зашифровать),\nа дальше сколько угодно строк исходного текста\nДля расшифровки:\nв входной файл скопировать то, что получилось в результате работы программы\nт.е. на первой строке будет код (число) и 2 (расшифровать),\nа дальше строки зашифрованного текста\nВведите 1 для запуска, 0 для отмены\nПодтверждение: '
    v_info='В входном файле на первой строке код (заглавные и строчные латинские буквы,\nцифры, пробел и точка)\nа далее сколько угодно строк исходного (или зашифрованного) текста\nВведите 1 для запуска, 0 для отмены\nПодтверждение: '
    t_info='В входной файл записать сколько угодно строк текста,\nнаписанного на русском языке, но с английской раскладкой,\nто есть смотрим на русские клавишы клавиатуры, но печатаем английские буквы.\nЗнаки препинания, находящиеся на русских буквах, переводятся в буквы!\nНапример, запятая переводится в б, двоеточие - в ж и т.д.\nВведите 1 для запуска, 0 для отмены\nПодтверждение: '
    tabs_info='В входной файл скопировать программу на Питоне с отступами (по 4 пробела), чтобы их убрать\n(это полезно, чтобы зашифровать текст программы)\nили результат с убранными отступами, чтобы их восстановить\n(и потом просто поменять расширение файла с txt на py)\nВведите 1 для удаления отступов, 2 для восстановления, 0 для выхода\nПодтверждение: '
    error='Ошибка! Скорее всего, файл input.txt не создан или создан неправильно.\nФайл output.txt соответственно не создан, или пустой, или зашифрован до первой русской буквы\nВозврат назад'
    print('Для всех алгоритмов входной файл называется input и имеет расширение txt\nЭтот файл должен находиться в той же папке, из которой запущена данная программа\nВыходной файл output.txt автоматически создаётся в текущей папке\nРусские буквы не шифруются, их наличие в input.txt вызвовет ошибку!')
    print('0 = Выход (завершение работы)\n1 = Шифр Цезаря 2 = Шифр Вернама 3 = Раскладка\n4 = Убрать табуляцию')
    while True:
        print('Ваш выбор: ',end='')
        c=input()
        while c not in vvod_1:
            print('Некорректный ввод. Повторите: ',end='')
            c=input()
        c=int(c)
        if c==0:
            break
        elif c==1:
            print(c_info,end='')
            ch=input()
            while ch not in vvod_2:
                print('Некорректный ввод. Повторите: ',end='')
                ch=input()
            ch=int(ch)
            if ch:
                try:
                    cezar.crypto()
                    print('Результат в output.txt')
                except:
                    print(error)
            else:
                print('Возврат назад')
        elif c==2:
            print(v_info,end='')
            ch=input()
            while ch not in vvod_2:
                print('Некорректный ввод. Повторите: ',end='')
                ch=input()
            ch=int(ch)
            if ch:
                try:
                    vernam.crypto()
                    print('Результат в output.txt')
                except:
                    print(error)
            else:
                print('Возврат назад')
        elif c==3:
            print(t_info,end='')
            ch=input()
            while ch not in vvod_2:
                print('Некорректный ввод. Повторите: ',end='')
                ch=input()
            ch=int(ch)
            if ch:
                try:
                    translite.crypto()
                    print('Результат в output.txt')
                except:
                    print(error)
            else:
                print('Возврат назад')
        elif c==4:
            print(tabs_info,end='')
            ch=input()
            while ch not in (vvod_2+['2']):
                print('Некорректный ввод. Повторите: ',end='')
                ch=input()
            ch=int(ch)
            if ch:
                try:
                    if ch==1:
                        tabs.crypto()
                    else:
                        tabs.decrypto()
                    print('Результат в output.txt')
                except:
                    print(error)
            else:
                print('Возврат назад')
        else:
            print('Небольшая пасхалка)')
            nebag.s4et()

print('До свидания!\nНажмите ENTER, чтобы закрыть',end='')
input()

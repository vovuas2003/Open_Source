with open("out", "r") as a, open("nonparallel_out", "r") as b:
    a1 = a.readlines()
    b1 = b.readlines()
    sa = a1.pop(0)
    sb = b1.pop(0)
    t1 = a1.pop()
    t2 = b1.pop()
    f = (a1 == b1)
print()
print(sb, t2)
print(sa, t1)
print("Is equal: ", f)
print()

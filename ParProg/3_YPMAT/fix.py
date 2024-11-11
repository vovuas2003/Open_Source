f = open("fix.txt", "r")
d = f.readlines()
f.close()
if d[0][0] == 't' and d[-1][0] == 'E':
    se = d.pop(0)
    ti = d.pop()
else:
    for i in range(len(d)):
        if d[i][0] == 't':
            se = d.pop(i)
            break
    for i in range(len(d)):
        if d[i][0] == 'E':
            ti = d.pop(i)
            break
for i in range(len(d)):
    d[i] = d[i].split(":")
    d[i][0] = int(d[i][0])
    d[i][1] = int(d[i][1])
    d[i][2] = d[i][2][: -1]
d = sorted(d, key = lambda x: (x[0], x[1]))
f = open("out", "w")
f.write(se)
se = se.split()
t = float(se[0][2 :])
nt = int(se[1][3 :])
h = float(se[2][2 :])
nh = int(se[3][3 :])
np = int(se[4][3 :])
for t in range(nt):
    for p in range(np - 1):
        f.write(d[t * np + p][2] + " ")
    f.write(d[(t + 1) * np - 1][2] + "\n")
f.write(ti)
f.close()

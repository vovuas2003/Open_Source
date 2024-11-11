import matplotlib.pyplot as plt
import numpy

N = 5

f = open("to_plot", "r")
se = f.readline().split()
t = float(se[0][2 :])
nt = int(se[1][3 :])
h = float(se[2][2 :])
nh = int(se[3][3 :])
np = int(se[4][3 :])
x = [h * i for i in range(nh)]
y = []
for i in range(nt):
    y.append(list(map(float, f.readline().split())))
time = float(f.readline().split()[3])
f.close()

frames = numpy.linspace(0, nt - 1, N)
plt.figure(figsize = (13.5, 6.5))
plt.title("np = " + str(np) + ", time = " + str(time) + " sec")
plt.grid()
plt.xlim(0, h * nh)
for i in frames:
    plt.plot(x, y[int(i)])
plt.show()

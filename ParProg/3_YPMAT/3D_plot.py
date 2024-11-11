import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

f = open("to_plot", "r")
se = f.readline().split()
t = float(se[0][2 :])
nt = int(se[1][3 :])
h = float(se[2][2 :])
nh = int(se[3][3 :])
x = [h * i for i in range(nh)]
arr = []
for i in range(nt):
    arr.append(list(map(float, f.readline().split())))
time = float(f.readline().split()[3])
f.close()

t_L = 0
t_R = t * nt
x_L = 0
x_R = h * nh
t = np.linspace(t_L, t_R, nt)
x = np.linspace(x_L, x_R, nh)

fig = plt.figure(figsize=(7, 4))
ax_3d = fig.add_subplot(projection='3d')

ax_3d.set_xlabel('x')
ax_3d.set_ylabel('t')
ax_3d.set_zlabel('z')

xgrid, tgrid = np.meshgrid(x, t)
zgrid = np.array(arr)
ax_3d.plot_wireframe(xgrid, tgrid, zgrid)

plt.show()

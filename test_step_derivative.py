from rkf45 import RK
import numpy as np
import matplotlib.pyplot as plt


def F(x, y):
    return 1 * np.logical_and(x >= 0, x <= 1) + 0


npts_rk = 4
npts_euler = 16
rk = RK(F, np.array([-1]), np.array([0]), 3 / npts_rk, npts_rk)
x_values, y_values = rk.solve()
euler = RK(F, np.array([-1]), np.array([0]), 3 / npts_euler, npts_euler, order_method=1)
x_euler, y_euler = euler.solve()

x = np.linspace(-1, 2, 1000)
f = np.cumsum(F(x, 0)) * (x[1] - x[0])

plt.plot(x, f, "-", label="True")
plt.plot(x_values, y_values, "-o", label=f"RK4 {npts_rk}pts")
plt.plot(x_euler, y_euler, "-x", label=f"Explicit Euler {npts_euler}pts")
plt.legend()
plt.show()

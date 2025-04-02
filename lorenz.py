from rkf45 import RK
import numpy as np
import matplotlib.pyplot as plt


def lorenz(x, t, s=10.0, r=28.0, b=8.0 / 3.0):
    x, y, z = x
    xx = s * (y - x)
    yy = r * x - y - x * z
    zz = x * y - b * z
    return np.array([xx, yy, zz])


ts = np.linspace(0, 100, 10000)
x0 = np.array([0, 1, 1.05])
rk = RK(lorenz, 0.0, x0, 1e-2, 10000)

sol = rk.solve()

# plt.plot(ts, sol)

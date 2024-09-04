from typing import Tuple, Union, Callable
import numpy as np
from numpy import typing
from collections.abc import Sequence


class RK(object):
    """
    Class that solves
    dy/dx = f(y, x)
    y(x0) = y0
    """

    def __init__(
        self,
        fn: Callable[Tuple[Union[np.double, float]], Tuple[Union[np.double, float]]],
        x_init: typing.ArrayLike,
        y_init: typing.ArrayLike,
        step_size: Union[np.double, float],
        n_steps: int,
        *,
        order_method=4,
        dtype: Union[typing.DTypeLike, typing.DTypeLike] = float
    ) -> None:
        self.fn = fn
        self.x_init = x_init
        self.y_init = y_init
        self.step_size = self.h = step_size
        self.N = self.n = n_steps

        self.order = order_method
        if isinstance(y_init, Sequence):
            self.n_eq = len(y_init)
        else:
            self.n_eq = 1

        self.dtype = dtype
        self.a, self.b, self.c = self.compute_butcher_table()

    def compute_butcher_table(self):
        # Define coefficients for a specific RK method (e.g., classical RK4)
        if self.order == 1:
            self.a = a = [[0]]
            self.b = b = [1]
            self.c = c = [0]
            return a, b, c
        if self.order == 4:
            self.a = a = [
                [0.0, 0.0, 0.0, 0.0],
                [0.5, 0.0, 0.0, 0.0],
                [0.0, 0.5, 0.0, 0.0],
                [0.0, 0.0, 1.0, 0.0],
            ]
            self.b = b = [1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0]
            self.c = c = [0.0, 0.5, 0.5, 1.0]
            return a, b, c
        else:
            raise NotImplementedError(
                "Butcher table not defined for the specified order."
            )

    def compute_k(self, x, y):
        # a, b, c = self.a, self.b, self.c
        # step_size = h = self.step_size
        # order = self.order
        # n_eq = self.n_eq
        k = np.zeros((self.order, self.n_eq))
        k[0] = self.fn(x, y)
        for j in range(1, self.order):
            k[j] = self.fn(
                x + self.c[j] * self.step_size, y + self.step_size * k[j - 1]
            )

        return np.dot(self.b, k)

    def solve(self, *, p_bar=False, debug=False):
        y = self.y_init
        x = self.x_init
        if debug:
            print(x)

        if p_bar:
            import tqdm

            pbar = tqdm.tqdm(
                np.linspace(self.x_init, self.step_size * self.N + self.x_init, self.N)
            )
        else:
            pbar = np.linspace(
                self.x_init, self.step_size * self.N + self.x_init, self.N
            )
        y_values = np.zeros((self.N + 1, self.n_eq))
        x_values = np.zeros((self.N + 1, self.n_eq))

        y_values[0] = y
        x_values[0] = x

        for i, x in enumerate(pbar):
            y = y + self.step_size * self.compute_k(x, y)
            y_values[i + 1] = y
            x_values[i + 1] = x + self.step_size
            # x += self.step_size
            if debug:
                print(x)

        return x_values, y_values

    def iterate(self, x, y):
        """
        Does 1 iteration of the RK solver
        """
        return y + self.step_size * self.compute_k(x, y)

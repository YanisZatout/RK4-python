#include "rk.h"
#include "tensor.h"
#include "tensor_empty_like.h"
#include "tensor_print.h"
#include <stdint.h>

ButcherTableau get_butcher_tableau(uint8_t order) {
  ButcherTableau bt = {0};

  // Forwards Euler
  static double a1_0[] = {0.};
  static double *a1[] = {a1_0};
  static double b1[] = {1.};
  static double c1[] = {0};
  // RK3
  static double a3_0[] = {0, 0, 0};
  static double a3_1[] = {0.5, 0, 0};
  static double a3_2[] = {-1.0, 2.0, 0.0};
  static double *a3[] = {a3_0, a3_1, a3_2};
  static double b3[] = {1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0};
  static double c3[] = {0, 1. / 2., 1.};
  // RK4
  static double a4_0[] = {0.0, 0.0, 0.0, 0.0};
  static double a4_1[] = {0.5, 0.0, 0.0, 0.0};
  static double a4_2[] = {0.0, 0.5, 0.0, 0.0};
  static double a4_3[] = {0.0, 0.0, 1.0, 0.0};
  static double *a4[] = {a4_0, a4_1, a4_2, a4_3};
  static double b4[] = {1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0};
  static double c4[] = {0.0, 0.5, 0.5, 1.0};

  switch (order) {
  case 1:
    bt.a = a1;
    bt.b = b1;
    bt.c = c1;
    bt.stages = 1;
    break;
  case 3:
    bt.a = a3;
    bt.b = b3;
    bt.c = c3;
    bt.stages = 3;
    break;
  case 4:
    bt.a = a4;
    bt.b = b4;
    bt.c = c4;
    bt.stages = 4;
    break;
  // Add other methods as needed
  default:
    break;
  }
  return bt;
}

/*
 * Time stepping function, with timestep $\mathrm{d}t such that
 * we solve for $\frac{\mathrm{d}y}{\mathrm{d}t}=f(x, t, \cdots)$
 * state: Tensor *, the pointer to the x_n state
 * t: double, current time
 * dt: double, timestep to use
 * ode_func: function pointer to function that gives the derivative
 * of the current state at time $t$ such that we have y'=f(x,t, **args)
 * */
Tensor *rk_step(Tensor *state, double t, uint8_t order, double dt,
                Tensor *(*ode_func)(Tensor *, double, Tensor *), Tensor *args) {
  // Initialize the butcher tableau matrices and vectors a[][], b[] and c[]
  ButcherTableau butcher_tab = get_butcher_tableau(order);
  if (butcher_tab.stages == 0)
    return NULL;

  // Allocating the k[][] tensors that represent the coefficients
  // to add  up to get $y_{n+1}$ from $y_n$.
  Tensor **k = malloc(butcher_tab.stages * sizeof(Tensor *));
  for (uint8_t i = 0; i < butcher_tab.stages; i++) {
    k[i] = tensor_empty_like(state);
  }
  // Allocating copy of the current state $y_n$.
  Tensor *temp_state = tensor_empty_like(state);

  // Adding additional arguments for f(X, t, **args)

  for (uint8_t stage = 0; stage < butcher_tab.stages; stage++) {
    // Copying $y_n$ data into temp_state
    tensor_copy_data(state, temp_state);

    // Computing $y_{n, s}=y_n+\mathrm{d}t \sum_{j=0}^{s-2}a[s][j]k[j]$
    for (uint8_t j = 0; j < stage; j++) {
      tensor_axpy(butcher_tab.a[stage][j] * dt, k[j], temp_state);
    }

    // Computing $t_{n, s} = t_n + \mathrm{d}c[s]$
    double t_inter = t + butcher_tab.c[stage] * dt;

    // Computing derivative of the intermediate state (x_{n, s}, t_{n, s})
    // $y'=f(x_{n, s}, t_{n, s})$
    ode_func(temp_state, t_inter, args);

    // Copying data into k[s]
    tensor_copy_data(temp_state, k[stage]);
  }

  // Adding up $y_{n+1} = y_{n} + \mathrm{d}\sum_{j=0}^{s-1}b[j]k[j]
  for (uint8_t stage = 0; stage < butcher_tab.stages; stage++) {
    tensor_axpy(butcher_tab.b[stage] * dt, k[stage], state);
  }

  // Freeing up data
  tensor_free(temp_state);
  for (uint8_t i = 0; i < butcher_tab.stages; i++) {
    tensor_free(k[i]);
  }
  free(k);
  return state;
}

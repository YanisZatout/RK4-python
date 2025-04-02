#include "src/rk.h"
#include "src/tensor.h"
#include "src/tensor_save_load.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Tensor *lorenz(Tensor *X, double t, Tensor *args) {
  double s = tensor_get(args, 0);
  double r = tensor_get(args, 1);
  double b = tensor_get(args, 2);

  const double x = tensor_get(X, 0);
  const double y = tensor_get(X, 1);
  const double z = tensor_get(X, 2);

  double xp, yp, zp;
  xp = s * (y - x);
  yp = r * x - y - x * z;
  zp = x * y - b * z;

  tensor_set(X, xp, 0);
  tensor_set(X, yp, 1);
  tensor_set(X, zp, 2);
  return X;
}

int main(void) {

  double dt = 0.01;
  double t = 0.0;
  const double s = 10.0, r = 28.0, b = 8.0 / 3.0;
  int Nt = 10000;
  uint8_t order = 4;

  uint32_t dims_state[] = {3}, dims_states[] = {Nt, 4};
  Tensor *state = tensor_alloc(1, dims_state, ROW_MAJOR);
  Tensor *states = tensor_alloc(2, dims_states, ROW_MAJOR);

  uint32_t dims_hyper_params[] = {3};
  Tensor *srb = tensor_alloc(1, dims_hyper_params, ROW_MAJOR);
  tensor_set(srb, s, 0);
  tensor_set(srb, r, 1);
  tensor_set(srb, b, 2);

  tensor_set(state, 0.0, 0);  // x0
  tensor_set(state, 1.0, 1);  // y0
  tensor_set(state, 1.05, 2); // z0

  tensor_set(states, 0.0, 0, 0);  // t0
  tensor_set(states, 0.0, 0, 1);  // x0
  tensor_set(states, 1.0, 0, 2);  // y0
  tensor_set(states, 1.05, 0, 3); // z0

  t += dt;

  for (int i = 1; i < Nt; i++) {
    state = rk_step(state, t, order, dt, lorenz, srb);
    double x = tensor_get(state, 0);
    double y = tensor_get(state, 1);
    double z = tensor_get(state, 2);
    tensor_set(states, t, i, 0);
    tensor_set(states, x, i, 1);
    tensor_set(states, y, i, 2);
    tensor_set(states, z, i, 3);
    t += dt;
  }

  tensor_savetxt("solution.txt", states, "%18e", 0, 0, "Time,X,Y,Z", "", "# ");

  tensor_set(state, 0.0, 0);  // x0
  tensor_set(state, 1.0, 1);  // y0
  tensor_set(state, 1.05, 2); // z0

  tensor_free(state);
  tensor_free(states);

  return 0;
}

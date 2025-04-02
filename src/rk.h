#pragma once
#include "stdint.h"
#include "tensor.h"
#include "tensor_copy.h"
#include "tensor_empty_like.h"
#include "tensor_op.h"
#include "tensor_print.h"

typedef struct {
  double **a;
  double *b;
  double *c;
  uint8_t stages;
} ButcherTableau;

Tensor *rk_step(Tensor *state, double t, uint8_t order, double dt,
                Tensor *(*ode_func)(Tensor *, double, Tensor *), Tensor *args);

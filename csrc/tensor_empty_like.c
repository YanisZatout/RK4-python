#include "tensor_empty_like.h"

Tensor *tensor_empty_like(Tensor *X) {
  Tensor *t = malloc(sizeof(Tensor));
  t->ndim = X->ndim;
  t->order = X->order;
  t->dims = malloc(X->ndim * sizeof(uint32_t));
  t->strides = malloc(X->ndim * sizeof(uint32_t));

  // Calculate total size and copy dimensions
  t->size = 1;
  for (int i = 0; i < X->ndim; i++) {
    t->dims[i] = X->dims[i];
    t->size *= X->dims[i];
  }

  // Calculate strides
  if (X->order == ROW_MAJOR) {
    t->strides[X->ndim - 1] = 1;
    for (int i = X->ndim - 2; i >= 0; i--)
      t->strides[i] = t->strides[i + 1] * t->dims[i + 1];
  } else { // COL_MAJOR
    t->strides[0] = 1;
    for (int i = 1; i < X->ndim; i++)
      t->strides[i] = t->strides[i - 1] * t->dims[i - 1];
  }

  t->data = malloc(t->size * sizeof(double));
  return t;
}

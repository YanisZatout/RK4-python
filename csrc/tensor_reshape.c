#include "tensor_reshape.h"

bool tensor_reshape(Tensor *t, uint8_t new_ndim, const uint32_t *new_dims) {
  // Calculate product of new dimensions
  uint32_t product = 1;
  for (uint8_t i = 0; i < new_ndim; i++) {
    if (new_dims[i] == 0)
      return false;
    product *= new_dims[i];
  }

  // Verify size matches
  if (product != t->size) {
    return false;
  }

  // Free old dimension and stride arrays
  free(t->dims);
  free(t->strides);

  // Allocate new dimension array
  t->dims = malloc(new_ndim * sizeof(uint32_t));
  if (!t->dims)
    return false;
  memcpy(t->dims, new_dims, new_ndim * sizeof(uint32_t));

  // Allocate and calculate new strides
  t->strides = malloc(new_ndim * sizeof(uint32_t));
  if (!t->strides) {
    free(t->dims);
    return false;
  }

  // Calculate strides based on storage order
  if (t->order == ROW_MAJOR) {
    t->strides[new_ndim - 1] = 1;
    for (int i = new_ndim - 2; i >= 0; i--) {
      t->strides[i] = t->strides[i + 1] * t->dims[i + 1];
    }
  } else { // COLUMN_MAJOR
    t->strides[0] = 1;
    for (uint8_t i = 1; i < new_ndim; i++) {
      t->strides[i] = t->strides[i - 1] * t->dims[i - 1];
    }
  }

  t->ndim = new_ndim;
  return true;
}

int test_reshape() {
  uint32_t shape[] = {27};
  Tensor *t_col = tensor_alloc(1, shape, COL_MAJOR);
  Tensor *t_row = tensor_alloc(1, shape, ROW_MAJOR);

  tensor_print(t_col, 2);
  tensor_print(t_row, 2);

  uint32_t new_dims[] = {3, 3, 3};
  tensor_reshape(t_col, 3, new_dims);
  tensor_reshape(t_row, 3, new_dims);

  tensor_print(t_col, 2);
  tensor_print(t_row, 2);

  tensor_free(t_col);
  tensor_free(t_row);
  return 0;
}

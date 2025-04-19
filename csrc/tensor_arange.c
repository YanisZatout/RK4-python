#include "tensor_arange.h"

bool tensor_arange(Tensor *t, double start, double stop, double step) {
  if (step == 0.0)
    return false; // Step cannot be zero
  if (t->size == 0)
    return false; // Empty tensor

  // Validate direction
  if ((step > 0.0 && stop <= start) || (step < 0.0 && stop >= start)) {
    return false;
  }

  // Calculate the number of elements and fill the tensor
  uint32_t n = 0;
  for (; n < t->size; n++) {
    const double value = start + n * step;
    if ((step > 0.0 && value >= stop) || (step < 0.0 && value <= stop)) {
      break; // Exceeds bounds
    }
    t->data[n] = value;
  }

  // Check if all elements were filled correctly
  if (n != t->size)
    return false;

  // Verify the next value would exceed the bounds
  const double next_value = start + t->size * step;
  if ((step > 0.0 && next_value < stop) || (step < 0.0 && next_value > stop)) {
    return false; // Tensor is too small
  }

  return true;
}

int test_arange() {

  uint32_t shape[] = {27};
  Tensor *t_col = tensor_alloc(1, shape, COL_MAJOR);
  Tensor *t_row = tensor_alloc(1, shape, ROW_MAJOR);

  // Fill with sample data
  // for (int i = 0; i < 27; i++) {
  //   t->data[i] = i + 1;
  // }
  tensor_print(t_col, 2);
  tensor_print(t_row, 2);

  tensor_arange(t_col, 0, 27, 1.);
  tensor_arange(t_row, 0, 27, 1.);

  tensor_print(t_col, 2);
  tensor_print(t_row, 2);

  tensor_free(t_col);
  tensor_free(t_row);
  return 0;
}

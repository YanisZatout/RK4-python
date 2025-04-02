#include "tensor.h"
#include <stdint.h>
/*
 * Addition
 */
bool tensor_add_unsafe(const Tensor *a, const Tensor *b, Tensor *result) {
  for (uint32_t i = 0; i < result->size; i++) {
    result->data[i] = a->data[i] + b->data[i];
  }
  return true;
}

/*
 * Substraction
 */
bool tensor_sub_unsafe(const Tensor *a, const Tensor *b, Tensor *result) {
  for (uint32_t i = 0; i < result->size; i++) {
    result->data[i] = a->data[i] - b->data[i];
  }
  return true;
}

/*
 * Substraction
 */
bool tensor_mul_unsafe(const Tensor *a, const Tensor *b, Tensor *result) {
  for (uint32_t i = 0; i < result->size; i++) {
    result->data[i] = a->data[i] * b->data[i];
  }
  return true;
}

/*
 * Division
 */
bool tensor_div_unsafe(const Tensor *a, const Tensor *b, Tensor *result) {
  for (uint32_t i = 0; i < result->size; i++) {
    result->data[i] = a->data[i] / b->data[i];
  }
  return true;
}
// y = a*x + y
bool tensor_axpy(double a, const Tensor *x, Tensor *y) {
  for (uint32_t i = 0; i < y->size; i++){
    y->data[i] = a * x->data[i] + y->data[i] ;
  }
  return true;
}

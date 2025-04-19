#include "tensor_copy.h"
#include "tensor.h"
#include <stdint.h>

Tensor *tensor_copy(const Tensor *src) {
  Tensor *dest = tensor_alloc(src->ndim, src->dims, src->order);
  for (uint32_t i = 0; i < src->size; i++)
    dest->data[i] = src->data[i];
  return dest;
}

void tensor_copy_data(const Tensor *src, Tensor *dest){
  for (uint32_t i = 0; i < src->size; i++)
    dest->data[i] = src->data[i];
}

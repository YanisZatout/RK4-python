#include "tensor.h"

// Helper function to calculate element offset
uint32_t calculate_offset(const Tensor *t, const uint32_t *indices) {
  uint32_t offset = 0;
  for (int d = 0; d < t->ndim; d++) {
    offset += indices[d] * t->strides[d];
  }
  return offset;
}

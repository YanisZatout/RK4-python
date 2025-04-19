#include "tensor.h"

// Memory management
Tensor *tensor_alloc(uint8_t ndim, uint32_t *dims, StorageOrder order) {
  Tensor *t = malloc(sizeof(Tensor));
  t->ndim = ndim;
  t->order = order;
  t->dims = malloc(ndim * sizeof(uint32_t));
  t->strides = malloc(ndim * sizeof(uint32_t));

  // Calculate total size and copy dimensions
  t->size = 1;
  for (int i = 0; i < ndim; i++) {
    t->dims[i] = dims[i];
    t->size *= dims[i];
  }

  // Calculate strides
  if (order == ROW_MAJOR) {
    t->strides[ndim - 1] = 1;
    for (int i = ndim - 2; i >= 0; i--)
      t->strides[i] = t->strides[i + 1] * t->dims[i + 1];
  } else { // COL_MAJOR
    t->strides[0] = 1;
    for (int i = 1; i < ndim; i++)
      t->strides[i] = t->strides[i - 1] * t->dims[i - 1];
  }

  t->data = calloc(t->size, sizeof(double));
  return t;
}

/*
 * Free everything
 */
void tensor_free(Tensor *t) {
  free(t->data);
  free(t->dims);
  free(t->strides);
  free(t);
}

/*
 * get the (x_1, x_2, \cdots, x_n) element
 */
double tensor_get(const Tensor *t, ...) {
  va_list ap;
  va_start(ap, t);
  uint32_t indices[t->ndim];

  for (int i = 0; i < t->ndim; i++)
    indices[i] = va_arg(ap, uint32_t);

  va_end(ap);

  uint32_t offset = 0;
  for (int i = 0; i < t->ndim; i++) {
    if (indices[i] >= t->dims[i]) {
      fprintf(stderr, "Index out of bounds\n");
      return 0;
    }
    offset += indices[i] * t->strides[i];
  }
  return t->data[offset];
}

/*
 * Setter function
 */
void tensor_set(Tensor *t, double value, ...) {
  va_list ap;
  va_start(ap, value);
  uint32_t indices[t->ndim];

  for (int i = 0; i < t->ndim; i++)
    indices[i] = va_arg(ap, uint32_t);

  va_end(ap);

  uint32_t offset = 0;
  for (int i = 0; i < t->ndim; i++) {
    if (indices[i] >= t->dims[i]) {
      fprintf(stderr, "Index out of bounds\n");
      return;
    }
    offset += indices[i] * t->strides[i];
  }
  t->data[offset] = value;
}

// Broadcasting helper
int broadcast_shapes(const Tensor *a, const Tensor *b, uint32_t *out_shape) {
  uint8_t max_ndim = (a->ndim > b->ndim) ? a->ndim : b->ndim;
  uint32_t a_dims[max_ndim], b_dims[max_ndim];

  // Pad with 1s for broadcasting
  memset(a_dims, 1, max_ndim * sizeof(uint32_t));
  memset(b_dims, 1, max_ndim * sizeof(uint32_t));

  memcpy(a_dims + (max_ndim - a->ndim), a->dims, a->ndim * sizeof(uint32_t));
  memcpy(b_dims + (max_ndim - b->ndim), b->dims, b->ndim * sizeof(uint32_t));

  // Calculate broadcasted shape
  for (int i = 0; i < max_ndim; i++) {
    if (a_dims[i] != b_dims[i] && a_dims[i] != 1 && b_dims[i] != 1) {
      return -1; // Incompatible shapes
    }
    out_shape[i] = (a_dims[i] > b_dims[i]) ? a_dims[i] : b_dims[i];
  }
  return max_ndim;
}

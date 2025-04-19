#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { ROW_MAJOR, COL_MAJOR } StorageOrder;

typedef struct {
  double *data;      // Flattened data array
  uint32_t *dims;    // Dimension sizes
  uint32_t *strides; // Stride for each dimension
  uint8_t ndim;      // Number of dimensions
  uint32_t size;     // Total number of elements
  StorageOrder order;
} Tensor;

Tensor *tensor_alloc(uint8_t ndim, uint32_t *dims, StorageOrder order);
void tensor_free(Tensor *t);
void tensor_set(Tensor *t, double value, ...);
double tensor_get(const Tensor *t, ...);

#pragma once
#include "tensor.h"
#include "tensor_print.h"

bool tensor_reshape(Tensor *t, uint8_t new_ndim, const uint32_t *new_dims);
int test_reshape();

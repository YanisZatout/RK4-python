#pragma once
#include "tensor.h"
#include "tensor_utils.h"
void tensor_print(const Tensor *t, int edge_items);

#define TENSOR_PRINT(TENSOR) tensor_print((TENSOR), 2)

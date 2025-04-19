#pragma once
#include "tensor.h"
/*
 * Tensor deepcopy
 * */

Tensor *tensor_copy(const Tensor *src);
void tensor_copy_data(const Tensor *src, Tensor *dest);

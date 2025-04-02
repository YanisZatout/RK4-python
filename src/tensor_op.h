#pragma once
#include "tensor.h"

bool tensor_add_unsafe(const Tensor *a, const Tensor *b, Tensor *result);
bool tensor_sub_unsafe(const Tensor *a, const Tensor *b, Tensor *result);
bool tensor_mul_unsafe(const Tensor *a, const Tensor *b, Tensor *result);
bool tensor_div_unsafe(const Tensor *a, const Tensor *b, Tensor *result);
bool tensor_axpy(double a, const Tensor *x, Tensor *y);

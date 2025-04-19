#pragma once
#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>
void tensor_savetxt(const char *fname, const Tensor *X, const char *fmt,
                    char delimiter, char newline, const char *header,
                    const char *footer, const char *comments);

#include "tensor_print.h"

// Recursive printing function with truncation
static void print_recursive(const Tensor *t, uint32_t *indices, int current_dim,
                            uint32_t *edge_items, int depth) {
  const int spaces_per_level = 2;

  if (current_dim == t->ndim) {
    // Print actual element value
    uint32_t offset = calculate_offset(t, indices);
    printf("%.6g", t->data[offset]);
    return;
  }

  const uint32_t dim_size = t->dims[current_dim];
  const bool truncate = (dim_size > (*edge_items * 2));

  printf("[");
  if (current_dim < t->ndim - 1)
    printf("\n%*s", (depth + 1) * spaces_per_level, "");

  for (uint32_t i = 0; i < dim_size; i++) {
    if (truncate && i == *edge_items && dim_size > *edge_items * 2) {
      printf("..., ");
      if (current_dim < t->ndim - 1)
        printf("\n%*s", (depth + 1) * spaces_per_level, "");
      i = dim_size - *edge_items;
    }

    indices[current_dim] = i;
    print_recursive(t, indices, current_dim + 1, edge_items, depth + 1);

    if (i != dim_size - 1) {
      printf(", ");
      if (current_dim < t->ndim - 1)
        printf("\n%*s", (depth + 1) * spaces_per_level, "");
    }
  }

  if (current_dim < t->ndim - 1)
    printf("\n%*s", depth * spaces_per_level, "");
  printf("]");
}

// Main print function
void tensor_print(const Tensor *t, int edge_items) {
  if (t->ndim == 0) {
    printf("Scalar: %.6g\n", t->data[0]);
    return;
  }

  uint32_t indices[t->ndim];
  // printf("Tensor shape: (");
  // for (int d = 0; d < t->ndim; d++) {
  //   printf("%u", t->dims[d]);
  //   if (d != t->ndim - 1)
  //     printf(", ");
  // }
  // printf(")\n");
  uint32_t edge_items_uint = (uint32_t)edge_items;
  print_recursive(t, indices, 0, &edge_items_uint, 0);
  printf("\n");
}

#include "tensor_save_load.h"

void tensor_savetxt(const char *fname, const Tensor *X, const char *fmt,
                    char delimiter, char newline, const char *header,
                    const char *footer, const char *comments) {
  // Set defaults if NULL/0 is provided
  if (!fmt)
    fmt = "%.18e";
  if (delimiter == 0)
    delimiter = ' ';
  if (newline == 0)
    newline = '\n';
  if (!comments)
    comments = "# ";
  if (!header)
    header = "";
  if (!footer)
    footer = "";

  FILE *fp = fopen(fname, "w");
  if (!fp) {
    perror("Error while opening file in tensor_savetxt");
    return;
  }
  // Write header
  if (strlen(header) > 0) {
    const char *start = header;
    const char *end;
    while ((end = strchr(start, '\n')) != NULL) {
      fprintf(fp, "%s%.*s%c", comments, (int)(end - start), start, newline);
      start = end + 1;
    }
    if (*start)
      fprintf(fp, "%s%s%c", comments, start, newline);
  }

  // Write data
  if (X->size > 0) {
    // Calculate elements per row (first dimension is row count)
    size_t elements_per_row = 1;
    for (uint8_t i = 1; i < X->ndim; i++) {
      elements_per_row *= X->dims[i];
    }

    // Create array to store indices for non-contiguous tensors
    uint32_t *indices = calloc(X->ndim, sizeof(uint32_t));
    size_t current_element = 0;

    for (size_t row = 0; row < (X->ndim > 0 ? X->dims[0] : 1); row++) {
      if (X->ndim > 0)
        indices[0] = row;

      for (size_t col = 0; col < elements_per_row; col++) {
        // Calculate element offset using strides
        uint32_t offset = 0;
        for (uint8_t d = 0; d < X->ndim; d++) {
          offset += indices[d] * X->strides[d];
        }

        // Write element
        fprintf(fp, fmt, X->data[offset]);

        // Update indices for next element
        if (X->ndim > 1) {
          for (uint8_t d = X->ndim - 1; d >= 1; d--) {
            if (++indices[d] < X->dims[d])
              break;
            indices[d] = 0;
          }
        }

        // Write delimiter unless last element
        if (col < elements_per_row - 1)
          fprintf(fp, "%c", delimiter);
      }
      fprintf(fp, "%c", newline);
      current_element += elements_per_row;
    }
    free(indices);
    // Write footer
    if (strlen(footer) > 0) {
      const char *start = footer;
      const char *end;
      while ((end = strchr(start, '\n')) != NULL) {
        fprintf(fp, "%s%.*s%c", comments, (int)(end - start), start, newline);
        start = end + 1;
      }
      if (*start)
        fprintf(fp, "%s%s%c", comments, start, newline);
    }

    fclose(fp);
  }
}

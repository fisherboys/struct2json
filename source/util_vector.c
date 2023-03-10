#include "util_vector.h"

#include <string.h>
#include <stdlib.h>

#define VECTOR_MAX_COUNT 10240 /* large enough for demo */
#define VECTOR_GROW_STRIDE 16

void vector_init(vector_t *arr)
{
  arr->count = 0;
  arr->allocated = 0;
  arr->values = NULL;
}

int vector_push_back(vector_t *arr, const void *elems, int elem_size, int nelems)
{
  int allocated;

  if (arr->count >= VECTOR_MAX_COUNT || nelems >= VECTOR_MAX_COUNT || nelems + arr->count >= VECTOR_MAX_COUNT) {
    return -1;
  }

  allocated = arr->allocated;
  while (allocated < (int)(arr->count + nelems)) {
    allocated += VECTOR_GROW_STRIDE;
  }

  if (allocated > arr->allocated) {
    void *new_values = malloc(elem_size * allocated);
    if (new_values == NULL) {
      /* no memory */
      return -1;
    }

    if (arr->count > 0) {
      memcpy(new_values, arr->values, elem_size * arr->count);
    }

    if (arr->allocated > 0) {
      free(arr->values);
    }

    arr->values = new_values;
    arr->allocated = allocated;
  }

  if (nelems > 0) {
    memcpy((char *)arr->values + (elem_size * arr->count), elems, elem_size * nelems);
    arr->count += nelems;
  }

  return 0;
}
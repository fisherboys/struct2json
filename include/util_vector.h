#ifndef __UTIL_VECTOR_H__
#define __UTIL_VECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int count;
  int allocated;
  void *values;
} vector_t;

void vector_init(vector_t *arr);
int vector_push_back(vector_t *arr, const void *elems, int elem_size, int nelems);
/* vector_fini should be relized by yourself, as element maybe pointer to heap */

#ifdef __cplusplus
}
#endif

#endif /* __UTIL_VECTOR_H__ */
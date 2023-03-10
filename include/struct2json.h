#ifndef __STRUCT_2_JSON_H__
#define __STRUCT_2_JSON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cJSON.h"

#define rela_addr(type, field) (&((type *)0)->field)

typedef enum {
  OBJ_TYPE_VOID,
  OBJ_TYPE_INT8, /* bool, char... */
  OBJ_TYPE_INT32,
  OBJ_TYPE_DOUBLE,
  OBJ_TYPE_STRING,
  OBJ_TYPE_STRUCT,
  OBJ_TYPE_VECTOR,
} obj_type_e;

typedef struct obj_type_info {
  obj_type_e obj_type;
  char *obj_name;
  uint32_t obj_size;
  uint32_t array_size;
  void *obj_addr;
  const struct obj_type_info *child;
} obj_type_info_t;

#define OBJ_TYPE_STRUCT_END { .obj_type = OBJ_TYPE_VOID }

/**
 * @brief Initialize object, set all values to 0 or NULL.
 * 
 * @param obj_info: The description of object.
 * @param obj_addr: The pointer to object.
 */
void cjson_init(const obj_type_info_t *obj_info, void *obj_addr);

/**
 * @brief Serialize struct data to json string.
 * 
 * @param obj_info: The description of object.
 * @param obj_addr: The pointer to object.
 * @return NULL: Serialized failed.
 *         NOT NULL: Serialized json string, must free later, or it would be memory leak.
 */
char* cjson_serialize(const obj_type_info_t *obj_info, const void *obj_addr);

/**
 * @brief Deserialize json string to struct data.
 * 
 * @param json_str: Json string.
 * @param obj_info: The description of object.
 * @param obj_addr: The pointer to object.
 * @return 0: SUCCESS
 *         <0: FAILURE
 */
int cjson_deserialize(const char *json_str, const obj_type_info_t *obj_info, void *obj_addr);

/**
 * @brief Release resouces when serializing or deserializing.
 * 
 * @param obj_info: The description of object.
 * @param obj_addr: The pointer to object.
 */
void cjson_fini(const obj_type_info_t *obj_info, const void *obj_addr);


/* Utils functions below */
/**
 * @brief strdup 
 */
char *cjson_strdup(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* __STRUCT_2_JSON_H__ */
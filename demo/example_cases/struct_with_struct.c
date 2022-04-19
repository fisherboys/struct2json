#include "struct_with_number.h"

#include <stdio.h>
#include <stdlib.h>
#include "struct2json.h"

/* original structure data */
typedef struct s2j_case3_sub_struct {
  int8_t value_int;
  char *value_str;
} s2j_case3_sub_struct_t;

typedef struct s2j_case3 {
  s2j_case3_sub_struct_t sub_value;
  int32_t value_int32;
} s2j_case3_t;

/* Step1. should define the obj_type_info of target struct */
static const obj_type_info_t s2j_case3_sub_struct_struct_type[] = {
  {
          .obj_type = OBJ_TYPE_INT8,
          .obj_name = "ValueInt",
          .obj_addr = rela_addr(s2j_case3_sub_struct_t, value_int),
  },
  {
          .obj_type = OBJ_TYPE_STRING,
          .obj_name = "ValueStr",
          .obj_addr = rela_addr(s2j_case3_sub_struct_t, value_str),
  },
  OBJ_TYPE_STRUCT_END
};

static const obj_type_info_t s2j_case3_struct_subtypes[] = { {
                                                                     .obj_type = OBJ_TYPE_STRUCT,
                                                                     .obj_name = "SubStruct",
                                                                     .obj_size = sizeof(s2j_case3_t),
                                                                     .obj_addr = rela_addr(s2j_case3_t, sub_value),
                                                                     .child = s2j_case3_sub_struct_struct_type,
                                                             },
                                                             {
                                                                     .obj_type = OBJ_TYPE_INT32,
                                                                     .obj_name = "ValueInt32",
                                                                     .obj_addr = rela_addr(s2j_case3_t, value_int32),
                                                             },
                                                             OBJ_TYPE_STRUCT_END };

const obj_type_info_t s2j_case3_struct_type = {
  .obj_type = OBJ_TYPE_STRUCT,
  .obj_name = NULL,
  .obj_size = sizeof(s2j_case3_t),
  .obj_addr = NULL,
  .child = s2j_case3_struct_subtypes,
};

/**
 * {
 *   "SubStruct": {
 *     "ValueInt": 5,
 *     "ValueStr": "Hello struct2json"
 *   },
 *   "ValueInt32": 128
 * }
 */
void example_case_struct_with_struct(void)
{
  s2j_case3_t data = { 0 };
  s2j_case3_t result = { 0 };
  char *json_str = NULL;

  printf("==============================\n");
  printf("===========Case 3=============\n");
  /* Step2. Initialize */
  s2j_init(&s2j_case3_struct_type, &data);
  s2j_init(&s2j_case3_struct_type, &result);

  /* Must malloc heap to take string, and s2j_fini will relase memory */
  data.sub_value.value_str = s2j_strdup("Hello struct2json");
  data.sub_value.value_int = 5;
  data.value_int32 = 128;
  /* should check whether malloc success here, but example just skip it */

  /* Step3. serialize or deserialize */
  json_str = s2j_serialize(&s2j_case3_struct_type, &data);
  if (NULL == json_str) {
    printf("Case3 serialize failed\n");
    return;
  }
  printf("Serialized data:\n%s\n", json_str);

  if (0 != s2j_deserialize(json_str, &s2j_case3_struct_type, &result)) {
    printf("Case3 deserialize failed\n");
    goto failure;
  }
  printf("Deserialized result:\nvalue_int[%d], value_str[%s], value_int32[%d]\n\n", result.sub_value.value_int,
         result.sub_value.value_str, result.value_int32);

failure:
  /* Step4. release */
  s2j_fini(&s2j_case3_struct_type, &data);
  s2j_fini(&s2j_case3_struct_type, &result);
  free(json_str);
}
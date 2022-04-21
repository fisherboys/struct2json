#include "struct_with_number.h"

#include <stdio.h>
#include <stdlib.h>
#include "struct2json.h"

/* original structure data */
typedef struct cjson_case2 {
  char *str;
} cjson_case2_t;

/* Step1. should define the obj_type_info of target struct */
static const obj_type_info_t cjson_case2_stuct_subtypes[] = {
  {
    .obj_type = OBJ_TYPE_STRING,
    .obj_name = "String",
    .obj_addr = rela_addr(cjson_case2_t, str),
  },
  OBJ_TYPE_STRUCT_END
};

const obj_type_info_t cjson_case2_struct_type = {
  .obj_type = OBJ_TYPE_STRUCT,
  .obj_name = NULL,
  .obj_size = sizeof(cjson_case2_t),
  .obj_addr = NULL,
  .child = cjson_case2_stuct_subtypes,
};


/**
 * {
 *   "String": "Hello struct2json",
 * }
 */
void example_case_struct_with_string(void) {
  cjson_case2_t data = { 0 };
  cjson_case2_t result = { 0 };
  char *json_str = NULL;

  printf("==============================\n");
  printf("===========Case 2=============\n");
  /* Step2. Initialize */
  cjson_init(&cjson_case2_struct_type, &data);
  cjson_init(&cjson_case2_struct_type, &result);

  /* Must malloc heap to take string, and cjson_fini will relase memory */
  data.str = cjson_strdup("Hello struct2json");
  /* should check whether malloc success here, but example just skip it */

  /* Step3. serialize or deserialize */
  json_str = cjson_serialize(&cjson_case2_struct_type, &data);
  if (NULL == json_str) {
    printf("Case2 serialize failed\n");
    return;
  }
  printf("Serialized data:\n%s\n", json_str);

  if (0 != cjson_deserialize(json_str, &cjson_case2_struct_type, &result)) {
    printf("Case2 deserialize failed\n");
    goto failure;
  }
  printf("Deserialized result:\nstring[%s]\n\n", result.str);

failure:
  /* Step4. release */
  cjson_fini(&cjson_case2_struct_type, &data);
  cjson_fini(&cjson_case2_struct_type, &result);
  free(json_str);
}
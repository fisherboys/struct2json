#include "struct_with_number.h"

#include <stdio.h>
#include <stdlib.h>
#include "struct2json.h"

/* original structure data */
typedef struct s2j_case2 {
  char *str;
} s2j_case2_t;

/* Step1. should define the obj_type_info of target struct */
static const obj_type_info_t s2j_case2_stuct_subtypes[] = {
  {
    .obj_type = OBJ_TYPE_STRING,
    .obj_name = "String",
    .obj_addr = rela_addr(s2j_case2_t, str),
  },
  OBJ_TYPE_STRUCT_END
};

const obj_type_info_t s2j_case2_struct_type = {
  .obj_type = OBJ_TYPE_STRUCT,
  .obj_name = NULL,
  .obj_size = sizeof(s2j_case2_t),
  .obj_addr = NULL,
  .child = s2j_case2_stuct_subtypes,
};


/**
 * {
 *   "String": "Hello struct2json",
 * }
 */
void example_case_struct_with_string(void) {
  s2j_case2_t data = { 0 };
  s2j_case2_t result = { 0 };
  char *json_str = NULL;

  printf("==============================\n");
  printf("===========Case 2=============\n");
  /* Step2. Initialize */
  s2j_init(&s2j_case2_struct_type, &data);
  s2j_init(&s2j_case2_struct_type, &result);

  /* Must malloc heap to take string, and s2j_fini will relase memory */
  data.str = s2j_strdup("Hello struct2json");
  /* should check whether malloc success here, but example just skip it */

  /* Step3. serialize or deserialize */
  json_str = s2j_serialize(&s2j_case2_struct_type, &data);
  if (NULL == json_str) {
    printf("Case2 serialize failed\n");
    return;
  }
  printf("Serialized data:\n%s\n", json_str);

  if (0 != s2j_deserialize(json_str, &s2j_case2_struct_type, &result)) {
    printf("Case2 deserialize failed\n");
    goto failure;
  }
  printf("Deserialized result:\nstring[%s]\n\n", result.str);

failure:
  /* Step4. release */
  s2j_fini(&s2j_case2_struct_type, &data);
  s2j_fini(&s2j_case2_struct_type, &result);
  free(json_str);
}
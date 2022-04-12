#include "struct_with_number.h"

#include <stdio.h>
#include <stdlib.h>
#include "struct2json.h"

/* original structure data */
typedef struct s2j_case1 {
  int8_t num1;
  int32_t num2;
  double num3;
} s2j_case1_t;

/* Step1. should define the obj_type_info of target struct */
static const obj_type_info_t s2j_case1_stuct_subtypes[] = {
  {
    .obj_type = OBJ_TYPE_INT8,
    .obj_name = "Int8",
    .obj_addr = rela_addr(s2j_case1_t, num1),
  },
  {
      .obj_type = OBJ_TYPE_INT32,
      .obj_name = "Int32",
      .obj_addr = rela_addr(s2j_case1_t, num2),
  },
  {
      .obj_type = OBJ_TYPE_DOUBLE,
      .obj_name = "Double",
      .obj_addr = rela_addr(s2j_case1_t, num3),
  },
  OBJ_TYPE_STRUCT_END
};

const obj_type_info_t s2j_case1_struct_type = {
  .obj_type = OBJ_TYPE_STRUCT,
  .obj_name = NULL,
  .obj_size = sizeof(s2j_case1_t),
  .obj_addr = NULL,
  .child = s2j_case1_stuct_subtypes,
};


/**
 * {
 *   "Int8": 1,
 *   "Int32": 2,
 *   "Double": 3.3
 * }
 */
void example_case_struct_with_number(void) {
  s2j_case1_t data = { 0 };
  s2j_case1_t result = { 0 };
  char *json_str = NULL;

  /* Step2. Initialize */
  s2j_init(&s2j_case1_struct_type, &data);
  s2j_init(&s2j_case1_struct_type, &result);

  data.num1 = 1;
  data.num2 = 2;
  data.num3 = 3.3;

  /* Step3. serialize or deserialize */
  json_str = s2j_serialize(&s2j_case1_struct_type, &data);
  if (NULL == json_str) {
    printf("Case1 serialize failed\n");
    return;
  }
  printf("Serialized data:\n%s\n", json_str);

  if (0 != s2j_deserialize(json_str, &s2j_case1_struct_type, &result)) {
    printf("Case1 deserialize failed\n");
    goto failure;
  }
  printf("Deserialized result:\nnum1[%d], num2[%d], num3[%lf]\n", result.num1, result.num2, result.num3);

failure:
  /* Step4. release */
  s2j_fini(&s2j_case1_struct_type, &data);
  s2j_fini(&s2j_case1_struct_type, &result);
  free(json_str);
}
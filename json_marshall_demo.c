#include <stdio.h>
#include <stdlib.h>

#define JSON_DEBUG 0
#if JSON_DEBUG
typedef struct data_level2 {
  int8_t num2_1;
  int32_t num2_2;
  double num2_3;
  char *string2;
} data_level2_t;

typedef struct data_level1 {
  int8_t num1_1;
  int32_t num1_2;
  double num1_3;
  char *string1;
  data_level2_t level2;
} data_level1_t;

typedef struct data_array {
  int32_t num_array;
} data_array_t;

const obj_type_info_t dynamic_array_struct_type = {
  .type_id = OBJ_TYPE_INT32,
  // .obj_name = "arr_num",
  .obj_size = sizeof(int32_t),
};

const obj_type_info_t dynamic_array_string_struct_type = {
  .type_id = OBJ_TYPE_STRING,
  .obj_size = sizeof(char *),
};

static const obj_type_info_t data_level2_struct_subtypes[] = {
  {
    .type_id = OBJ_TYPE_INT8,
    .obj_name = "Num2_1",
    .obj_addr = rela_addr(data_level2_t, num2_1),
  },
  {
      .type_id = OBJ_TYPE_INT32,
      .obj_name = "Num2_2",
      .obj_addr = rela_addr(data_level2_t, num2_2),
  },
  {
      .type_id = OBJ_TYPE_DOUBLE,
      .obj_name = "Num2_3",
      .obj_addr = rela_addr(data_level2_t, num2_3),
  },
  {
      .type_id = OBJ_TYPE_STRING,
      .obj_name = "String2",
      .obj_addr = rela_addr(data_level2_t, string2),
  },
  {
    .type_id = OBJ_TYPE_DYNAMIC_ARRAY,
    .obj_name = "Vector_string",
    .obj_addr = rela_addr(data_level2_t, vector_string),
    .child = &dynamic_array_string_struct_type,
  },
  OBJ_TYPE_STRUCT_END
};

static const obj_type_info_t data_level1_stuct_subtypes[] = {
  {
    .type_id = OBJ_TYPE_INT8,
    .obj_name = "Num1_1",
    .obj_addr = rela_addr(data_level1_t, num1_1),
  },
  {
      .type_id = OBJ_TYPE_INT32,
      .obj_name = "Num1_2",
      .obj_addr = rela_addr(data_level1_t, num1_2),
  },
  {
      .type_id = OBJ_TYPE_DOUBLE,
      .obj_name = "Num1_3",
      .obj_addr = rela_addr(data_level1_t, num1_3),
  },
  {
    .type_id = OBJ_TYPE_STRING,
    .obj_name = "String1",
    .obj_addr = rela_addr(data_level1_t, string1),
  },
  {
    .type_id = OBJ_TYPE_DYNAMIC_ARRAY,
    .obj_name = "Vector1",
    .obj_addr = rela_addr(data_level1_t, vector1),
    .child = &dynamic_array_struct_type,
  },
  {
      .type_id = OBJ_TYPE_STRUCT,
      .obj_name = "Level2",
      .obj_size = sizeof(data_level2_t),
      .obj_addr = rela_addr(data_level1_t, level2),
      .child = data_level2_struct_subtypes,
  },
  OBJ_TYPE_STRUCT_END
};

const obj_type_info_t data_level1_struct_type = {
  .type_id = OBJ_TYPE_STRUCT,
  .obj_name = "LEVEL1",
  .obj_size = sizeof(data_level1_t),
  .obj_addr = NULL,
  .child = data_level1_stuct_subtypes,
};
#else
typedef struct data_string {
  // ahpl_dynamic_array_t vector_string;
  int8_t num1_1;
  int32_t num1_2;
  double num1_3;
  char *string1;
  ahpl_dynamic_array_t vector_string;
} data_string_t;

const obj_type_info_t dynamic_array_string_struct_type = {
  .type_id = OBJ_TYPE_STRING,
  .obj_size = sizeof(char *),
};

static const obj_type_info_t data_string_subtypes[] = {
  {
    .type_id = OBJ_TYPE_INT8,
    .obj_name = "Num1_1",
    .obj_addr = rela_addr(data_string_t, num1_1),
    // .child = &dynamic_array_string_struct_type,
  },
  {
    .type_id = OBJ_TYPE_INT32,
    .obj_name = "Num1_2",
    .obj_addr = rela_addr(data_string_t, num1_2),
  },
  {
    .type_id = OBJ_TYPE_DOUBLE,
    .obj_name = "Num1_3",
    .obj_addr = rela_addr(data_string_t, num1_3),
  },
  {
    .type_id = OBJ_TYPE_STRING,
    .obj_name = "String1",
    .obj_addr = rela_addr(data_string_t, string1),
  },
  {
    .type_id = OBJ_TYPE_DYNAMIC_ARRAY,
    .obj_name = "Vector_string",
    .obj_addr = rela_addr(data_string_t, vector_string),
    .child = &dynamic_array_string_struct_type,
  },
  OBJ_TYPE_STRUCT_END
};

const obj_type_info_t data_string_struct_type = {
  .type_id = OBJ_TYPE_STRUCT,
  .obj_name = "data unmarshal",
  .obj_size = sizeof(data_string_t),
  .obj_addr = NULL,
  .child = data_string_subtypes,
};
#endif
int main() {
#if JSON_DEBUG
  data_level1_t data;
  data_array_t tmp;
  char *tmp3 = "1234567890";
  char *tmp2 = "0987654321";
  cJSON *root = cJSON_CreateObject();
  json_obj_init(&data_level1_struct_type, &data);
  data.num1_1 = 1;
  data.num1_2 = 2;
  data.num1_3 = 3;
  data.string1 = "String1 value";
  tmp.num_array = 5;
  ahpl_dynamic_array_add_elems(&data.vector1, &tmp, sizeof(data_array_t), 1);
  tmp.num_array = 6;
  ahpl_dynamic_array_add_elems(&data.vector1, &tmp, sizeof(data_array_t), 1);
  data.level2.num2_1 = 4;
  data.level2.num2_2 = 5;
  data.level2.num2_3 = 6;
  data.level2.string2 = NULL;
  printf("&tmp2: %p, &tmp3: %p\n", &tmp2, &tmp3);
  printf("tmp2: %p, tmp3: %p\n", tmp2, tmp3);
  ahpl_dynamic_array_add_elems(&data.level2.vector_string, &tmp2, sizeof(char *), 1);
  ahpl_dynamic_array_add_elems(&data.level2.vector_string, &tmp3, sizeof(char *), 1);
  printf("obj_size huawei: %d\n", (int)sizeof(data_array_t));
  json_obj_marshal(&data_level1_struct_type, &data, root);
  char *print = cJSON_Print(root);
  printf("%s\n", print);
  free(print);
  free(data.vector1.values);
  cJSON_Delete(root);
#else
  data_string_t data;
  data_string_t tmp;
  char *tmp3 = ahpl_strdup("0987654321");
  char *tmp2 = ahpl_strdup("1234567890");
  cJSON *root = cJSON_CreateObject();
  json_obj_init(&data_string_struct_type, &data);
  json_obj_init(&data_string_struct_type, &tmp);
  ahpl_dynamic_array_add_elems(&data.vector_string, &tmp3, sizeof(char *), 1);
  ahpl_dynamic_array_add_elems(&data.vector_string, &tmp2, sizeof(char *), 1);
  data.num1_1 = 8;
  data.num1_2 = 16;
  data.num1_3 = 32.5;
  data.string1 = ahpl_strdup("hello");
  printf("vector string count: %d\n", (int)data.vector_string.count);
  json_obj_marshal(&data_string_struct_type, &data, root);
  char *print = cJSON_Print(root);
  printf("%s\n", print);
  json_obj_unmarshal(&data_string_struct_type, &tmp, root);
  printf("num1_1: %d\n", tmp.num1_1);
  printf("num1_2: %d\n", tmp.num1_2);
  printf("num1_3: %f\n", tmp.num1_3);
  printf("string1: %s\n", tmp.string1);
  printf("vector cnt: %d\n", tmp.vector_string.count);
  printf("huawei[%s]\n", (*(char **)(tmp.vector_string.values)));
  printf("huawei2[%s]\n", (*((char **)(tmp.vector_string.values) + 1)));
  free(print);
  cJSON_Delete(root);
  json_obj_fini(&data_string_struct_type, &data);
  json_obj_fini(&data_string_struct_type, &tmp);
#endif
  return 0;
}

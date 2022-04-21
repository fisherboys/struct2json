#include "struct2json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _get_size(const obj_type_info_t *type, int *size)
{
  int obj_size = 0;

  switch (type->obj_type) {
  case OBJ_TYPE_VOID:
    obj_size = 0;
    break;
  case OBJ_TYPE_INT8:
    obj_size = 1;
    break;
  case OBJ_TYPE_INT32:
    obj_size = 4;
    break;
  case OBJ_TYPE_DOUBLE:
    obj_size = 8;
    break;
  case OBJ_TYPE_STRING:
    obj_size = sizeof(char *);
    break;
  case OBJ_TYPE_STRUCT:
    obj_size = type->obj_size;
    break;
  default:
    /* Unknown type */
    goto err_out_label;
  }

  if (size != NULL) {
    *size = obj_size;
  }

  return 0;

err_out_label:
  return -1;
}

/* strdup() isn't standard C. It is a POSIX extension. */
char *cjson_strdup(const char *s)
{
  size_t len = 0;
  char *new = NULL;
  if (s != NULL) {
    len = strlen(s);
    new = (char *)malloc(len + 1);
    if (new != NULL) {
      memcpy(new, s, len);
      new[len] = '\0';
    }

    return new;
  }

  return NULL;
}

static void _init_typed_obj(const obj_type_info_t *type, const void *typed_obj_p)
{
  void *this_obj_addr = NULL;

  this_obj_addr = (char *)typed_obj_p + (uintptr_t)type->obj_addr;
  switch (type->obj_type) {
  case OBJ_TYPE_INT8:
    *(uint8_t *)this_obj_addr = 0;
    break;
  case OBJ_TYPE_INT32:
    *(int32_t *)this_obj_addr = 0;
    break;
  case OBJ_TYPE_DOUBLE:
    *(double *)this_obj_addr = 0.0;
    break;
  case OBJ_TYPE_STRING:
    *(char **)this_obj_addr = NULL;
    break;
  case OBJ_TYPE_STRUCT:
    for (type = type->child; type->obj_type != OBJ_TYPE_VOID; type++) {
      _init_typed_obj(type, this_obj_addr);
    }
    break;
  default:
    break;
  }
}

void cjson_init(const obj_type_info_t *obj_info, void *obj_addr)
{
  _init_typed_obj(obj_info, obj_addr);
}

int _serialize(const obj_type_info_t *obj_info, const void *obj_addr, cJSON **json_obj)
{
  int ret = 0;
  double value_number = 0;
  const void *this_obj_addr = NULL;
  char *value_char = NULL;
  bool is_null = true;
  cJSON *root = *json_obj;
  cJSON *item = NULL;

  this_obj_addr = (uint8_t *)obj_addr + (uintptr_t)obj_info->obj_addr;

  switch (obj_info->obj_type) {
  case OBJ_TYPE_VOID:
    break;
  case OBJ_TYPE_INT8:
    value_number = *(int8_t *)this_obj_addr;
    cJSON_AddNumberToObject(root, obj_info->obj_name, value_number);
    break;
  case OBJ_TYPE_INT32:
    value_number = *(int32_t *)this_obj_addr;
    cJSON_AddNumberToObject(root, obj_info->obj_name, value_number);
    break;
  case OBJ_TYPE_DOUBLE:
    value_number = *(double *)this_obj_addr;
    cJSON_AddNumberToObject(root, obj_info->obj_name, value_number);
    break;
  case OBJ_TYPE_STRING:
    /* a 'logic bool' value is one byte value */
    value_char = *(char **)this_obj_addr;
    is_null = !!(value_char == NULL);

    if (is_null) {
      cJSON_AddNullToObject(root, obj_info->obj_name);
    } else {
      cJSON_AddStringToObject(root, obj_info->obj_name, value_char);
    }
    break;
  case OBJ_TYPE_STRUCT:
    if (obj_info->obj_name) {
      item = cJSON_AddObjectToObject(root, obj_info->obj_name);
    } else {
      item = root;
    }
    obj_info = obj_info->child;
    while (obj_info->obj_type != OBJ_TYPE_VOID) {
      ret = _serialize(obj_info, this_obj_addr, &item);
      if (ret < 0) {
        goto err_out_label;
      }
      ++obj_info;
    }
    break;
  default:
    break;
  }
  return 0;
err_out_label:
  return ret;
}

char *cjson_serialize(const obj_type_info_t *obj_info, const void *obj_addr)
{
  char *json_str = NULL;
  int ret = 0;
  cJSON *root = cJSON_CreateObject();
  if (NULL == root) {
    return NULL;
  }
  ret = _serialize(obj_info, obj_addr, &root);
  if (0 != ret) {
    goto err_out_label;
  }

  json_str = cJSON_Print(root);
  cJSON_Delete(root);
  return json_str;
err_out_label:
  cJSON_Delete(root);
  return NULL;
}

#define GET_ITEM_AND_CHECK                                                                                             \
  item = cJSON_GetObjectItem(root, obj_info->obj_name);                                                                \
  if (NULL == item) {                                                                                                  \
    break;                                                                                                             \
  }

int _deserialize(const obj_type_info_t *obj_info, void *obj_addr, const cJSON **json_obj)
{
  int ret = 0;
  void *this_obj_addr = NULL;
  const cJSON *root = *json_obj;
  const cJSON *item = NULL;
  void *p_value = NULL;

  this_obj_addr = (uint8_t *)obj_addr + (uintptr_t)obj_info->obj_addr;

  switch (obj_info->obj_type) {
  case OBJ_TYPE_VOID:
    break;
  case OBJ_TYPE_INT8:
    GET_ITEM_AND_CHECK
    *(int8_t *)this_obj_addr = (int8_t)cJSON_GetNumberValue(item);
    break;
  case OBJ_TYPE_INT32:
    GET_ITEM_AND_CHECK
    *(int32_t *)this_obj_addr = (int32_t)cJSON_GetNumberValue(item);
    break;
  case OBJ_TYPE_DOUBLE:
    GET_ITEM_AND_CHECK
    *(double *)this_obj_addr = (double)cJSON_GetNumberValue(item);
    break;
  case OBJ_TYPE_STRING:
    GET_ITEM_AND_CHECK
    p_value = cJSON_GetStringValue(item);
    if (p_value) {
      *(char **)this_obj_addr = cjson_strdup((char *)p_value);
    }
    break;
  case OBJ_TYPE_STRUCT:
    if (NULL == obj_info->obj_name) {
      item = root;
    } else {
      item = cJSON_GetObjectItem(root, obj_info->obj_name);
    }
    /* do not contain the obj */
    if (NULL == item) {
      break;
    }
    obj_info = obj_info->child;
    while (obj_info->obj_type != OBJ_TYPE_VOID) {
      ret = _deserialize(obj_info, this_obj_addr, &item);
      if (ret < 0) {
        goto err_out_label;
      }
      ++obj_info;
    }
    break;
  default:
    break;
  }
  return 0;
err_out_label:
  return ret;
}

int cjson_deserialize(const char *json_str, const obj_type_info_t *obj_info, void *obj_addr)
{
  int ret = 0;
  cJSON *root = NULL;
  if (NULL == json_str || NULL == obj_info || NULL == obj_addr) {
    return -1;
  }
  root = cJSON_Parse(json_str);
  if (NULL == root) {
    return -1;
  }
  ret = _deserialize(obj_info, obj_addr, (const cJSON **)&root);
  if (0 != ret) {
    goto err_out_label;
  }
  cJSON_Delete(root);
  return 0;
err_out_label:
  cJSON_Delete(root);
  return -1;
}

static void _fini_typed_obj(const obj_type_info_t *type, const void *typed_obj_p)
{
  void *p = NULL;
  void *this_obj_addr = NULL;

  this_obj_addr = (char *)typed_obj_p + (uintptr_t)type->obj_addr;
  switch (type->obj_type) {
  case OBJ_TYPE_STRING:
    p = *(char **)this_obj_addr;
    if (p != NULL) {
      free(p);
      *(char **)this_obj_addr = NULL;
    }
    break;
  case OBJ_TYPE_STRUCT:
    for (type = type->child; type->obj_type != OBJ_TYPE_VOID; type++) {
      _fini_typed_obj(type, this_obj_addr);
    }
    break;
  default:
    break;
  }
}

void cjson_fini(const obj_type_info_t *obj_info, const void *obj_addr)
{
  _fini_typed_obj(obj_info, obj_addr);
}
#ifndef _TREE_H
#define _TREE_H

#include "utils/list.h"
#include "oid.h"
//使用 leaf_t 描述版本库中的一个文件
typedef struct {
    char *path;
    oid_t oid;
} leaf_t;

//使用 tree_t 记录文件列表
typedef struct {
    oid_t oid;
    list_t leaf_list;
} tree_t;

extern tree_t *tree_new();
extern void tree_delete(tree_t *this);
extern tree_t *tree_read(char *path);
extern void tree_write(tree_t *this, char *path);
extern tree_t *tree_load(oid_t *oid);
extern void tree_store(tree_t *this, oid_t *oid);
extern void tree_add(tree_t *this, char *path);
extern void tree_remove(tree_t *this, char *path);
extern oid_t *tree_find(tree_t *this, char *path);

#endif

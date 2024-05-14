#include "utils/std.h"
#include "utils/list.h"
#include "utils/split.h"
#include "utils/file.h"
#include "utils/dir.h"
#include "config.h"
#include "blob.h"
#include "tree.h"

//创建一个新的文件
leaf_t *leaf_new(char *path, oid_t *oid)
{
    leaf_t *this = malloc(sizeof(leaf_t));
    this->path = strdup(path);
    this->oid = *oid;
    return this;
}

//删除文件
void leaf_delete(leaf_t *this)
{
    free(this->path);
    free(this);
}

//建立一颗空树
tree_t *tree_new()
{
    tree_t *this = malloc(sizeof(tree_t));
    list_init(&this->leaf_list);
    return this;
}

//删除树
void tree_delete(tree_t *this)
{
    while (!list_is_empty(&this->leaf_list)) {
        leaf_t *leaf = list_pop_head(&this->leaf_list);
        leaf_delete(leaf);
    }
    free(this);
}

/*
读取 tree 类型文件的一行
分割该行，把 path 和 oid 保存到 tree 中
*/
void tree_load_leaf(tree_t *this, char *line)
{
    char *word_table[2];
    split_string(line, " ", word_table);

    oid_t oid;
    oid_set(&oid, word_table[0]);
    char *path = word_table[1];
    leaf_t *leaf = leaf_new(path, &oid);
    list_push_tail(&this->leaf_list, leaf);
}

//根据 tree 的路径，读取 tree
tree_t *tree_read(char *path)
{
    tree_t *this = tree_new();
    FILE *f = fopen(path, "r");

    char line[LINE_SIZE];
    read_line(f, line);
    read_line(f, line);

    while (read_line(f, line))
        tree_load_leaf(this, line);

    fclose(f);
    return this;
}
//把 tree 保存到指定路径 path
void tree_write(tree_t *this, char *path)
{
    node_t *node;
    leaf_t *leaf;

    FILE *f = fopen(path, "w");
    write_line(f, "tree");
    write_line(f, "0");

    list_each (&this->leaf_list, node, leaf)
        fprintf(f, "%s %s\n", leaf->oid.data, leaf->path);
    fclose(f);
}
//根据 tree 的 oid，读取 tree
tree_t *tree_load(oid_t *oid)
{
    char path[PATH_MAX];
    get_oid_path(oid, path);

    tree_t *this = tree_read(path);
    return this;
}
/*根据 oid 计算对应的 path
    把 tree 保存到指定路径 path
*/
void tree_store(tree_t *this, oid_t *oid)
{
    char path[PATH_MAX];
    get_oid_path(oid, path);

    tree_write(this, path);
}

/*
遍历 tree 中所有的 leaf
如果 tree 中已经存在 path
   更新 oid
如果 tree 中不存在 path
   新增由 path 和 oid 构成的 leaf
*/
void tree_add_leaf(tree_t *this, char *path, oid_t *oid)
{
    printf("add %s\n", path);

    node_t *node;
    leaf_t *leaf;
    list_each (&this->leaf_list, node, leaf) {
        if (strcmp(leaf->path, path) == 0) {
            leaf->oid = *oid;
            return;
        }
    }
    
    leaf = leaf_new(path, oid);
    list_push_tail(&this->leaf_list, leaf);
}
/*
如果 path 是数据文件
    调用 tree_add_leaf
如果 path 是目录
    调用 tree_add_leaf
        把该目录加入到 tree 中
        OID_ZERO 表示该文件的类型是目录
    遍历该目录下的所有文件，递归调用 tree_add
*/
void tree_add(tree_t *this, char *path)
{
    if (!file_is_dir(path)) {
        oid_t oid;
        store_blob(path, &oid);
        tree_add_leaf(this, path, &oid);
        return;
    }

    oid_t oid;
    oid_set(&oid, OID_ZERO);
    tree_add_leaf(this, path, &oid);

    dir_t *dir = dir_open(path);
    entry_t entry;

    while (dir_read(dir, &entry)) {
        if (strcmp(entry.name, GIT_DIR) == 0)
            continue;
        tree_add(this, entry.path);
    }

    dir_close(dir);
}

/*
遍历 tree 中的所有 leaf
删除符合条件的 leaf
*/
void tree_remove(tree_t *this, char *path)
{
    node_t *node;
    leaf_t *leaf;

    list_each (&this->leaf_list, node, leaf) {
        if (strcmp(leaf->path, path) == 0) {
            printf("rm %s\n", path);
            node_unlink(node);
            node_delete(node);
            leaf_delete(leaf);
            return;
        }
    }

    fatal("file not found");
}

oid_t *tree_find(tree_t *this, char *path)
{
    node_t *node;
    leaf_t *leaf;

    list_each (&this->leaf_list, node, leaf) {
        if (strcmp(leaf->path, path) == 0)
            return &leaf->oid;
    }
    return NULL;
}

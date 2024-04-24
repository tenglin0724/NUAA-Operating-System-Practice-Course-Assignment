#include "utils/std.h"
#include "utils/file.h"
#include "utils/dir.h"
#include "cmd.h"
#include "config.h"
#include "tree.h"
#include "oid.h"
#include "index.h"

tree_t *index_load()
{
    tree_t *this = tree_read(GIT_INDEX_PATH);
    return this;
}

void index_store(tree_t *this)
{
    tree_write(this, GIT_INDEX_PATH);
}

//将index文件通过SHA算法得到文件名，放入到objects中
void index_dup(oid_t *oid)
{
    get_file_oid(GIT_INDEX_PATH, oid);
    char target_path[PATH_MAX];
    get_oid_path(oid, target_path);
    copy_file(GIT_INDEX_PATH, target_path);
}

void git_add(int argc, char *argv[])
{
    //从文件加载tree
    tree_t *tree=index_load();
    //从参数列表中选出路径
    char*path=argv[1];
    //将文件路径下的文件保存在树中
    tree_add(tree,path);
    //将树上文件记录在tree文件中
    index_store(tree);
    //释放树
    tree_delete(tree);
}
//没有删除缓存区中的文件
void git_rm(int argc, char *argv[])
{
    //从文件中加载tree
    tree_t *tree=index_load();
    //从参数列表中得到路径
    char*path =argv[1];
    //将文件从树上删除
    tree_remove(tree,path);
    //将树上文件记录保存在tree文件中
    index_store(tree);
    //释放树
    tree_delete(tree);
}

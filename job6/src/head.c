#include "utils/std.h"
#include "utils/file.h"
#include "config.h"
#include "head.h"

//从HEAD文件读取一行
void head_load(char *line)
{
    load_line(GIT_HEAD_PATH, line);
}
//存一行到HEAD
void head_store(char *line)
{
    store_line(GIT_HEAD_PATH, line);
}

//将HEAD中的内容加载如oid中
void head_load_oid(oid_t *oid)
{
    char line[LINE_SIZE];
    load_line(GIT_HEAD_PATH, line);
    oid_set(oid, line);
}

//将oid内容写入HEAD中
void head_store_oid(oid_t *oid)
{
    char line[LINE_SIZE];
    load_line(GIT_HEAD_PATH, line);
    store_line(GIT_HEAD_PATH, oid->data);
}

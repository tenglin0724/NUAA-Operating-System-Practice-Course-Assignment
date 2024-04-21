#include "utils/std.h"
#include "cmd.h"
void usage(){
    puts("DeltaGit Usage");
    puts("  dg init");

    puts("  dg cat-file -t oid      打印类型");
    puts("  dg cat-file -s oid      打印大小");
    puts("  dg cat-file -p oid      打印内容");

    puts("  dg add file             把文件加入到索引区");
    puts("  dg add dir              把目录下的文件加入到索引区");
    puts("  dg rm file              从索引区中删除文件");
    puts("  dg rm dir               从索引区中删除目录下的文件");
    puts("");

    puts("  dg commit -m msg");
    puts("  dg checkout oid");
    puts("  dg log");

    exit(EXIT_FAILURE);
}

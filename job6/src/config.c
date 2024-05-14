#include "utils/std.h"
#include "utils/file.h"
#include "oid.h"
#include "cmd.h"
#include "config.h"

//要实现的初始化
void git_init(int argc, char *argv[])
{
    // 调用mkdir函数创建目录
    if (mkdir(GIT_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
        perror("初始化失败");
    }
    //创建文件
    const char *tree="tree\n0\n";
    make_file(GIT_INDEX_PATH,tree);
    make_file(GIT_HEAD_PATH,"0000000000000000000000000000000000000000\n");
    if (mkdir(GIT_OBJECTS_DIR, 0777) != 0) {
        perror("初始化失败");
    }else{
        printf("Initialized empty Git repository\n");
    }
}

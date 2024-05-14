#include "utils/std.h"
#include "cmd.h"

//主函数
int main(int argc, char *argv[])
{
    //当命令行参数为1时，表示输入的格式不正确，调用显示用法函数
    if (argc == 1)
        usage();
    //否则，调用cmd.h中的函数，并让参数列表从第二个开始执行
    do_cmd(argc - 1, argv + 1);
    return 0;
}
